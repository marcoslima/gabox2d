#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "rs_codec.h"

namespace Net
{
    class IpcClient
    {
        std::unique_ptr<boost::asio::io_context> io_context_;
        std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
        std::vector<char> receive_buffer_;
        bool connected_ = false;
        std::thread client_thread_;
        const size_t BUFFER_SIZE = 4096;
        std::function <void(const std::string &)> onData;
        std::string _server_ip = "127.0.0.1";
        int _server_port = 9876;
        std::string _message_buffer;
        RsCodec rs_codec_;

        void attemptConnect(const std::shared_ptr<boost::asio::steady_timer> &timer);
        void handleRead(const boost::system::error_code &error, size_t bytes_transferred);

    public:
        explicit IpcClient(const std::string& server_ip, int server_port, const std::function<void(const std::string &)> &onData);
        ~IpcClient();
        void startClient();
    };
}
