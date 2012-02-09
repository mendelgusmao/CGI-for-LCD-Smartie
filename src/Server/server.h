#include "stdafx.h"
#include "queue.h"

#ifndef SERVER_H
#define SERVER_H

using boost::asio::ip::udp;

class server
{

public:
    
    boost::asio::io_service& _io_service;
    udp::socket _socket;
    queue _queue;    
    udp::endpoint _sender_endpoint;
    bool _add_and_run;
    enum { max_length = 1024 };
    char _data[max_length];    
    
    server(boost::asio::io_service& io_service, short port, bool add_and_run);
    void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);
    void handle_send_to(const boost::system::error_code& error, size_t bytes_sent);
    void receive();

};

#endif // SERVER_H
