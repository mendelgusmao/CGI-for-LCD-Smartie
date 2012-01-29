#include "stdafx.h"
#include "protocol.cpp"
#include "queue.cpp"
#include "command.cpp"

#ifndef SERVER_CPP
#define SERVER_CPP

using boost::asio::ip::udp;
using std::cout;
using std::endl;

class server
{
public:
    server(boost::asio::io_service& io_service, short port) :
        _io_service(io_service),
        _socket(io_service, udp::endpoint(udp::v4(), port)),
		_queue(io_service) {

        receive();

    }

    void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd) {
        if (!error && bytes_recvd > 0) {

			command cmd;
			string temp(_data);
			cmd = protocol::parse(temp);

			if (!cmd.is_malformed) {
				_queue.add(cmd);
				cmd = _queue.get(cmd.line());
				cout << "Response: '" << cmd.response << "'" << endl;
			}
			// else: malformed packet. nothing to do

			char response[max_length];
			strcpy_s(response, cmd.response.c_str());

            _socket.async_send_to(
                boost::asio::buffer(response, cmd.response.size()), 
                _sender_endpoint,
                boost::bind(
                    &server::handle_send_to, 
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

    void handle_send_to(const boost::system::error_code& error, size_t bytes_sent) {
        receive();
    }

public:

    void receive() {

		_socket.async_receive_from(
            boost::asio::buffer(_data, max_length), 
            _sender_endpoint,
            boost::bind(
                &server::handle_receive_from, 
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );

    }

    boost::asio::io_service& _io_service;
    udp::socket _socket;
    queue _queue;	
    udp::endpoint _sender_endpoint;
    enum { max_length = 1024 };
    char _data[max_length];

};

#endif // SERVER_CPP
