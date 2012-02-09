#include "stdafx.h"
#include "queue.h"
#include "command.h"
#include "utils.h"

using boost::lexical_cast;

queue::queue(boost::asio::io_service& io_service) : 
  _timer(io_service, boost::posix_time::seconds(1)) {

    _timer.async_wait(boost::bind(&queue::run, this));

}

void queue::add(command &cmd, bool add_and_run) {

    map<string, command>::iterator it = _commands.find(cmd.line());

    if (it == _commands.end()) {
        cmd.response = "";
        _commands[cmd.line()] = cmd;
    
        if (add_and_run) {
            _commands[cmd.line()].run();
        }
    }
    else {
        _commands[cmd.line()].cleanup_timer = 0;
    }
}

void queue::run() {

    map<string, command>::iterator it;
    command cmd;
    int queue_size = _commands.size();

    _timer.expires_at(_timer.expires_at() + boost::posix_time::seconds(1));
    _timer.async_wait(boost::bind(&queue::run, this));

    std::cout << "Running queue (" << queue_size << ")" << std::endl;

    string title(lexical_cast<string>(queue_size));
    title = "CGI4LCD - " + title + " command" + (queue_size > 1 ? "s" : "") + " in queue";
    SetConsoleTitle(utils::s2ws(title).c_str());

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

command queue::get(const string &line) {
    return _commands[line];
}