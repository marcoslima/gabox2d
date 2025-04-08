#pragma once
#include <boost/asio.hpp>

using namespace std;


class GaServer
{
    using vec_socket_t = vector<shared_ptr<boost::asio::ip::tcp::socket>>;

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    vec_socket_t clients_;
    mutex clients_mutex_;
    thread server_thread_;

    string _compose_message(const string& data);

public:
    GaServer();
    ~GaServer();
    void startAccept();
    void broadcastStatus(const string &data);
    [[nodiscard]] bool isReady() const;
};
