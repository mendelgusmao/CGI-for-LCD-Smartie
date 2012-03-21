#include "stdafx.h"
#include "Server.h"
#include "Protocol.h"
#include "Queue.h"
#include "Command.h"

using namespace std;

Server::Server(boost::asio::io_service& io_service, short port, unsigned int max_threads) :
    _io_service(io_service),
    _socket(io_service, udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port)),
    _queue(io_service, max_threads)
    {

    receive();

}

void Server::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd) {
    if (!error && bytes_recvd > 0) {

        Command command;
        string temp(_data, bytes_recvd);
        command = Protocol::parse(temp);

        if (!command.is_malformed) {

            if (command.do_not_queue) {
                _queue.run(command);
            }
            else {
                _queue.add(command);
                command = _queue.get(command.line());
            }

        }
        // else: malformed packet. nothing to do

        char response[max_length];
        strcpy_s(response, command.response.c_str());

        _socket.async_send_to(
            boost::asio::buffer(response, command.response.size()), 
            _sender_endpoint,
            boost::bind(
                &Server::handle_send_to, 
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
    }
    else {
        receive();
    }
}

void Server::handle_send_to(const boost::system::error_code& error, size_t bytes_sent) {
    receive();
}

void Server::receive() {

    _socket.async_receive_from(
        boost::asio::buffer(_data, max_length), 
        _sender_endpoint,
        boost::bind(
            &Server::handle_receive_from, 
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );

}