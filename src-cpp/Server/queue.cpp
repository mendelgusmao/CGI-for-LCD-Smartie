#include "stdafx.h"
#include "command.cpp"
#include <map>

#ifndef QUEUE_CPP
#define QUEUE_CPP

using std::string;
using std::map;

class queue {
public:
    void add_function(command &cmd) {
        if (_commands.count(cmd.line()) == 0) {
            _commands[cmd.line()] = cmd;
            // start timer
        }
        else {
            _commands[cmd.line()].cleanup_timer = 0;
        }
    }

    void run_commands() {
        map<string, command>::iterator it;
        command cmd;

        for (it = _commands.begin(); it != _commands.end(); ++it) {
            cmd = it->second;

            cmd.timer++;
            cmd.cleanup_timer++;

            if (cmd.cleanup_timer >= cmd.timeout) {
                _commands.erase(it);
            }
            else if (cmd.timer >= cmd.interval) {
                cmd.execute();
                cmd.timer = 0;
            }
        }
    }

private:
    map<string, command> _commands;
    // timer ...
};

#endif // QUEUE_CPP
