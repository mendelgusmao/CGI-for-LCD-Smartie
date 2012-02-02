#include "stdafx.h"
#include "command.cpp"

#ifndef QUEUE_CPP
#define QUEUE_CPP

using std::string;
using std::map;

class queue {
public:

    queue(boost::asio::io_service& io_service) : 
      _timer(io_service, boost::posix_time::seconds(1)) {

        _timer.async_wait(boost::bind(&queue::run, this));

    }

    void add(command &cmd) {

        map<string, command>::iterator it = _commands.find(cmd.line());

        if (it == _commands.end()) {
            _commands[cmd.line()] = cmd;
            _commands[cmd.line()].run();
        }
        else {
            _commands[cmd.line()].cleanup_timer = 0;
        }
    }

    void run() {

        map<string, command>::iterator it;
        command cmd;

        _timer.expires_at(_timer.expires_at() + boost::posix_time::seconds(1));
        _timer.async_wait(boost::bind(&queue::run, this));

        std::cout << "Running queue (" << _commands.size() << ")" << std::endl;

        for (it = _commands.begin(); it != _commands.end(); ++it) {
            cmd = it->second;

            cmd.timer += 1000;
            cmd.cleanup_timer += 1000;

            std::cout << 
                "Command '" << cmd.line() << "'" << std::endl <<
                "Cleanup/Timeout: " << cmd.cleanup_timer << "/" << cmd.timeout << std::endl <<
                "Timer/Interval: " << cmd.timer << "/" << cmd.interval << std::endl <<
                "Cached Response: '" << cmd.response << "'" << std::endl;

            if (cmd.cleanup_timer >= cmd.timeout) {
                std::cout << "Erasing '" << cmd.line() << "'" << std::endl;
                _commands.erase(it);
                return;
            }
            else if (cmd.timer >= cmd.interval) {
                std::cout << "Running '" << cmd.line() << "'" << std::endl;
                cmd.run();
                cmd.timer = 0;
                
                std::cout << cmd.response << std::endl;
            }

            _commands[cmd.line()] = cmd;
        }

    }

    command get(string line) {
        return _commands[line];
    }

private:
    boost::asio::deadline_timer _timer;
    map<string, command> _commands;
};

#endif // QUEUE_CPP
