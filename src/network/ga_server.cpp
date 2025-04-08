#include <network/ga_server.h>
#include <iostream>
#include <thread>
#include <network/common.h>

using namespace boost::asio::ip;


GaServer::GaServer()
    : acceptor_(io_context_, tcp::endpoint(tcp::v4(), 9876))
    , rs_codec_(RS_CHUNK_SIZE, RS_REDUNDANCY)
{
    startAccept();
    server_thread_ = std::thread([this]
    {
        try
        {
            io_context_.run();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Server error: " << e.what() << std::endl;
        }
    });
}

GaServer::~GaServer()
{
    acceptor_.close();
    if (server_thread_.joinable())
    {
        server_thread_.join();
    }
}

void GaServer::startAccept()
{
    auto socket = std::make_shared<tcp::socket>(io_context_);
    acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code &error)
    {
        if (!error)
        {
            std::lock_guard lock(clients_mutex_);
            clients_.push_back(socket);
        }
        startAccept();
    });
}

string GaServer::compose_message_(const string& data)
{
    auto rs_encoded = rs_codec_.encode(data);
    auto message = string(rs_encoded.begin(), rs_encoded.end());
    message.insert(0, START_OF_MESSAGE);
    message.append(END_OF_MESSAGE);

    return std::move(message);
}

void GaServer::broadcastStatus(const string &data)
{
    std::lock_guard lock(clients_mutex_);
    auto message = compose_message_(data);
    for (auto it = clients_.begin(); it != clients_.end();)
    {
        try
        {
            boost::asio::write(**it, boost::asio::buffer(message));
            ++it;
        }
        catch (const std::exception &)
        {
            it = clients_.erase(it);
        }
    }
}

bool GaServer::isReady() const
{
    return acceptor_.is_open();
}
