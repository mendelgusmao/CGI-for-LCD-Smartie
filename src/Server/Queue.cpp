#include "stdafx.h"
#include "Queue.h"
#include "Command.h"
#include "Utils.h"

using boost::lexical_cast;

Queue::Queue(boost::asio::io_service& io_service, unsigned int max_threads) : 
  _timer(io_service, boost::posix_time::seconds(1)),
  _max_threads(max_threads),
  _running_threads(0) {

    _timer.async_wait(boost::bind(&Queue::process, this));

}

void Queue::add(Command &command) {

    map<string, Command>::iterator it = _commands.find(command.line());
    time_t now;
    time(&now);

    if (it == _commands.end()) {
        command.response = "";
        command.last_request = now;

        _commands[command.line()] = command;
    
        if (command.add_and_run) {
            run(command);
        }
    }
    else {
        _commands[command.line()].last_request = now;
    }
}

void Queue::process() {

    map<string, Command>::iterator it;
    Command command;

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
        command = it->second;

        time_t now;
        time(&now);

        echo("Command '" << command.line() << "'");
        echo("Cleanup Time: " << command.last_request << " + " << command.timeout);
        echo("Next Execution: " << command.last_execution << " + " << command.interval);
        echo("Cached Response: '" << command.response << "'");

        if (now >= command.last_request + command.timeout) {
            echo("Erasing '" << command.line() << "'");

            _commands.erase(it);
            break;
        }
        else if (_running_threads < _max_threads && command.is_running == false && now >= command.last_execution + command.interval) {
            boost::thread runner(boost::bind(&Queue::run, this, command));
        }

        _commands[command.line()] = command;
    }

}

Command Queue::get(const string &line) {
    return _commands[line];
}

void Queue::run(Command &command) {

    char psBuffer[128];
    FILE *iopipe;

    echo("Running '" << command.line() << "'");

    ++_running_threads;

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

    --_running_threads;
}
