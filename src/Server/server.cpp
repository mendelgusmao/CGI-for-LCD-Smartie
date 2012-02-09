#include "server.h"
#include "protocol.h"
#include "queue.h"
#include "command.h"

using namespace std;
using boost::asio::ip::udp;

server::server(boost::asio::io_service& io_service, short port, bool add_and_run) :
    _io_service(io_service),
    _socket(io_service, udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port)),
    _queue(io_service),
    _add_and_run(add_and_run)
    {

    receive();

}

void server::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd) {
    if (!error && bytes_recvd > 0) {

        command cmd;
        string temp(_data, bytes_recvd);
        cmd = protocol::parse(temp);

        if (!cmd.is_malformed) {
            if (cmd.do_not_queue) {
                cmd.run();
            }
            else {
                _queue.add(cmd, _add_and_run);
                cmd = _queue.get(cmd.line());
            }
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

void server::handle_send_to(const boost::system::error_code& error, size_t bytes_sent) {
    receive();
}

void server::receive() {

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