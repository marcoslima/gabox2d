#pragma once
#include <boost/asio.hpp>
#include <ga_ipc.h>

class GaServer
{
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> clients_;
    std::mutex clients_mutex_;
    std::thread server_thread_;

public:
    GaServer();
    ~GaServer();
    void startAccept();
    void broadcastStatus(const ipc::GaStatus& status);
    bool isReady() const
    {
        return acceptor_.is_open();
    }
};