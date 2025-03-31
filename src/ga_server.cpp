// In ga_server.cpp
#include "ga_server.h"

#include <iostream>
#include <thread>

GaServer::GaServer()
    : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9876))
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
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
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

void GaServer::broadcastStatus(const ipc::GaStatus &status)
{
    std::string data = ipc::GaStatusSerializer::serializeGaStatus(status);
    std::lock_guard lock(clients_mutex_);

    for (auto it = clients_.begin(); it != clients_.end();)
    {
        try
        {
            boost::asio::write(**it, boost::asio::buffer(data));
            ++it;
        }
        catch (const std::exception &)
        {
            it = clients_.erase(it);
        }
    }
}
