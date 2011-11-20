#include "stdafx.h"
#include <msgpack.hpp>

using std::string;

#ifndef COMMAND_CPP
#define COMMAND_CPP

class command {
public:
	string header;
    string executable;
	string arguments;
	int timeout;
	int interval;
	int timer;
	int cleanup_timer;
	std::string response;
	bool is_malformed;

	void execute() {
		response = line();
	}

    string line() {
        return executable + " " + arguments;
    }

	MSGPACK_DEFINE(header, executable, arguments, timeout, interval);
};

#endif // COMMAND_CPP
