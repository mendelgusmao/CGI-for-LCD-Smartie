#include "stdafx.h"

using std::string;

#ifndef COMMAND_CPP
#define COMMAND_CPP

class command {
public:
    string executable;
	string arguments;
	int timeout;
	int interval;
	int timer;
	int cleanup_timer;
	string response;
	bool is_malformed;

    string line() {
        return executable + " " + arguments;
    }

};

#endif 



