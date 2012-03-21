#include "stdafx.h"
#include "Queue.h"
#include "Command.h"
#include "Utils.h"

using boost::lexical_cast;

Queue::Queue(boost::asio::io_service& io_service) : 
  _timer(io_service, boost::posix_time::seconds(1)) {

    _timer.async_wait(boost::bind(&Queue::process, this));

}

void Queue::add(Command &cmd) {

    map<string, Command>::iterator it = _commands.find(cmd.line());
    time_t now;
    time(&now);

    if (it == _commands.end()) {
        cmd.response = "";
        cmd.last_request = now;

        _commands[cmd.line()] = cmd;
    
        if (cmd.add_and_run) {
            run(cmd);
        }
    }
    else {
        _commands[cmd.line()].last_request = now;
    }
}

void Queue::process() {

    map<string, Command>::iterator it;
    Command cmd;

    _timer.expires_at(_timer.expires_at() + boost::posix_time::seconds(1));
    _timer.async_wait(boost::bind(&Queue::process, this));

#ifdef DEBUG
    int queue_size = _commands.size();
    echo("Running queue (" << queue_size << ")");

    string title(lexical_cast<string>(queue_size));
    title = "CGI4LCD - " + title + " command" + (queue_size > 1 ? "s" : "") + " in queue";
    SetConsoleTitle(Utils::s2ws(title).c_str());
#endif

    for (it = _commands.begin(); it != _commands.end(); ++it) {
        cmd = it->second;

        time_t now;
        time(&now);

        echo("Command '" << cmd.line() << "'");
        echo("Cleanup Time: " << cmd.last_request << " + " << cmd.timeout);
        echo("Next Execution: " << cmd.last_execution << " + " << cmd.interval);
        echo("Cached Response: '" << cmd.response << "'");

        if (now >= cmd.last_request + cmd.timeout) {
            echo("Erasing '" << cmd.line() << "'");

            _commands.erase(it);
            break;
        }
        else if (cmd.is_running == false && now >= cmd.last_execution + cmd.interval) {
            boost::thread runner(boost::bind(&Queue::run, this, cmd));
        }

        _commands[cmd.line()] = cmd;
    }

}

Command Queue::get(const string &line) {
    return _commands[line];
}

void Queue::run(Command &command) {

    char psBuffer[128];
    FILE *iopipe;

    echo("Running '" << command.line() << "'");

    if (!command.do_not_queue) {
        _commands[command.line()].is_running = true;
    }

    iopipe = _popen(command.line().c_str(), "r");

    if (iopipe == NULL) {
        command.response = "[CGI4LCD] Error running...";
    }
    else {
        string response = "";

        while(!feof(iopipe)) {
            if(fgets(psBuffer, 128, iopipe) != NULL) {
                response += string(psBuffer);
            }
        }

        _pclose(iopipe);
        time(&command.last_execution);
        command.response = response;
    }

    echo("Runner response: '" << command.response << "'");

    if (!command.do_not_queue) {
        _commands[command.line()].is_running = false;
        _commands[command.line()] = command;
    }
}
