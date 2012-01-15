#include "stdafx.h"

using std::string;

#ifndef COMMAND_CPP
#define COMMAND_CPP

class command {
public:
    string executable;
	string arguments;
	unsigned int timeout;
	unsigned int interval;
	unsigned int timer;
	unsigned int cleanup_timer;
	string response;
	bool is_malformed;

	command() :
		timer(0),
		cleanup_timer(0)
	{}

    string line() {
        return executable + " " + arguments;
    }

    void run() {

		char psBuffer[128];
		FILE *iopipe;

		iopipe = _popen(line().c_str(), "r");

		if (iopipe == NULL) {
			response = "[CGI4LCD] Error running...";
		}
		else {
			response = "";

			while(!feof(iopipe)) {
				if(fgets(psBuffer, 128, iopipe) != NULL) {
					response += string(psBuffer);
				}
			}
			_pclose(iopipe);
		}
	}
};

#endif 



