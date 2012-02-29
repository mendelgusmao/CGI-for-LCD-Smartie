#include "stdafx.h"
#include "Queue.h"
#include "Command.h"
#include "Utils.h"

using boost::lexical_cast;

Queue::Queue(boost::asio::io_service& io_service) : 
  _timer(io_service, boost::posix_time::seconds(1)) {

    _timer.async_wait(boost::bind(&Queue::run, this));

}

void Queue::add(Command &cmd, bool add_and_run) {

    map<string, Command>::iterator it = _commands.find(cmd.line());

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

void Queue::run() {

    map<string, Command>::iterator it;
    Command cmd;
    int queue_size = _commands.size();

    _timer.expires_at(_timer.expires_at() + boost::posix_time::seconds(1));
    _timer.async_wait(boost::bind(&Queue::run, this));

#ifdef DEBUG
    echo("Running queue (" << queue_size << ")");

    string title(lexical_cast<string>(queue_size));
    title = "CGI4LCD - " + title + " command" + (queue_size > 1 ? "s" : "") + " in queue";
    SetConsoleTitle(Utils::s2ws(title).c_str());
#endif

    for (it = _commands.begin(); it != _commands.end(); ++it) {
        cmd = it->second;

        cmd.timer += 1000;
        cmd.cleanup_timer += 1000;

        echo("Command '" << cmd.line() << "'");
        echo("Cleanup/Timeout: " << cmd.cleanup_timer << "/" << cmd.timeout);
        echo("Timer/Interval: " << cmd.timer << "/" << cmd.interval);
        echo("Cached Response: '" << cmd.response << "'");

        if (cmd.cleanup_timer >= cmd.timeout) {
            echo("Erasing '" << cmd.line() << "'");

            _commands.erase(it);
            return;
        }
        else if (cmd.timer >= cmd.interval) {
            echo("Running '" << cmd.line() << "'");

            cmd.run();
            cmd.timer = 0;

            echo(cmd.response);

        }

        _commands[cmd.line()] = cmd;
    }

}

Command Queue::get(const string &line) {
    return _commands[line];
}