#include "stdafx.h"
#include "Queue.h"

#ifndef SERVER_H
#define SERVER_H

using boost::asio::ip::udp;

class Server
{

public:
    
    boost::asio::io_service& _io_service;
    udp::socket _socket;
    Queue _queue;    
    udp::endpoint _sender_endpoint;
    enum { max_length = 1024 };
    char _data[max_length];    
    
    Server(boost::asio::io_service& io_service, short port, unsigned int max_threads);
    void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);
    void handle_send_to(const boost::system::error_code& error, size_t bytes_sent);
    void receive();

};

#endif // SERVER_H
