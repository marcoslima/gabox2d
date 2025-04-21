#include <network/ipc_client.h>
#include <network/common.h>


namespace Net
{
    IpcClient::IpcClient(const std::string& server_ip, const int server_port, const std::function<void(const std::string &)> &onData)
        : onData(onData)
        , _server_ip(server_ip)
        , _server_port(server_port)
    {}

    IpcClient::~IpcClient()
    {
        // Stop the IO context to cancel any outstanding operations
        if (io_context_)
        {
            io_context_->stop();
        }

        // Close the socket
        if (socket_ && socket_->is_open())
        {
            boost::system::error_code ec;
            socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec); // NOLINT(*-unused-return-value)
            socket_->close(ec); // NOLINT(*-unused-return-value)
        }

        // Wait for the client thread to finish
        if (client_thread_.joinable())
        {
            client_thread_.join();
        }
    }

    void IpcClient::startClient()
    {
        io_context_ = std::make_unique<boost::asio::io_context>();
        socket_ = std::make_unique<boost::asio::ip::tcp::socket>(*io_context_);
        connected_ = false;

        // Start a reconnection timer
        const auto timer = std::make_shared<boost::asio::steady_timer>(*io_context_, std::chrono::seconds(1));
        attemptConnect(timer);

        // Run IO context in separate thread
        client_thread_ = std::thread([this]
        {
            try
            {
                io_context_->run();
            }
            catch (const std::exception &e)
            {
                std::cerr << "Client error: " << e.what() << std::endl;
            }
        });
    }

    void IpcClient::attemptConnect(const std::shared_ptr<boost::asio::steady_timer> &timer)
    {
        try
        {
            socket_->connect(boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address::from_string(_server_ip), _server_port));
            connected_ = true;

            std::cout << "Connected to server" << std::endl;

            // Start async read
            receive_buffer_.resize(BUFFER_SIZE);
            socket_->async_read_some(
                boost::asio::buffer(receive_buffer_),
                [this]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2)
                {
                    handleRead(std::forward<T0>(PH1), std::forward<T1>(PH2));
                });
        }
        catch ([[maybe_unused]] const std::exception &e)
        {
            std::cerr << "Connection attempt failed, retrying in 1 second..." << std::endl;

            // Schedule reconnection attempt
            timer->expires_after(std::chrono::seconds(1));
            timer->async_wait([this, timer](const boost::system::error_code &error)
            {
                if (!error)
                {
                    attemptConnect(timer);
                }
            });
        }
    }

    void IpcClient::handleRead(const boost::system::error_code &error, const size_t bytes_transferred)
    {
        if (error) return;

        std::string data(receive_buffer_.begin(), receive_buffer_.begin() + static_cast<int>(bytes_transferred));
        _message_buffer.append(data);
        // std::cout << "message buffer len: " << _message_buffer.size() << std::endl;
        const auto end_pos = _message_buffer.rfind(END_OF_MESSAGE);
        if (end_pos != std::string::npos)
        {
            const auto start_pos = _message_buffer.rfind(START_OF_MESSAGE);
            if (start_pos != std::string::npos)
            {
                // Extract the message
                data = _message_buffer.substr(start_pos + START_OF_MESSAGE.size(), end_pos - start_pos - START_OF_MESSAGE.size());
                _message_buffer.erase(0, end_pos + END_OF_MESSAGE.size());

                onData(data);
            }
            else
            {
                // No complete message found, clear the buffer
                _message_buffer.clear();
            }
        }

        // Continue reading
        const auto buffers = boost::asio::buffer(receive_buffer_);
        auto handler = [this]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2)
        {
            handleRead(std::forward<T0>(PH1), std::forward<T1>(PH2));
        };
        socket_->async_read_some(buffers, handler);
    }
}