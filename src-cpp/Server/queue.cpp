#include "stdafx.h"
#include "command.cpp"
#include "runner.cpp"
#include <map>

#ifndef QUEUE_CPP
#define QUEUE_CPP

using std::string;
using std::map;

class queue {
public:
    void add_function(command &cmd) {

        if (_commands.count(cmd.line()) == 0) {
			// start timer
		}

		map<string, command>::iterator it = _commands.find(cmd.line());

		if (it == _commands.end()) {
            _commands[cmd.line()] = cmd;
			std::cout << "command queued" << std::endl;
			runner rnr;
			rnr.run(cmd, "");
			std::cout << "command runned" << std::endl;
			std::cout << cmd.response << std::endl;
        }
        else {
            _commands[cmd.line()].cleanup_timer = 0;
        }
    }

    void run_commands() {
        map<string, command>::iterator it;
        command cmd;
		runner rnr;

        for (it = _commands.begin(); it != _commands.end(); ++it) {
            cmd = it->second;

            cmd.timer++;
            cmd.cleanup_timer++;

            if (cmd.cleanup_timer >= cmd.timeout) {
                _commands.erase(it);
            }
            else if (cmd.timer >= cmd.interval) {

				rnr.run(cmd, "");
                cmd.timer = 0;
				std::cout << cmd.response << std::endl;
            }
        }
    }

private:
    map<string, command> _commands;
    // timer ...
};

#endif // QUEUE_CPP
