#include "stdafx.h"

#ifndef RUNNER_CPP
#define RUNNER_CPP

#define BUFFER_SIZE 512

#include "command.cpp"

class runner {

public:
    void run(command &cmd, const string dir) {

		char psBuffer[128];
		FILE *iopipe;

		string cmd_line = cmd.executable + " " + cmd.arguments;

		iopipe = _popen(cmd_line.c_str(), "r");

		if (iopipe == NULL ) {
			cmd.response = "[CGI4LCD] Error running...";
		}
		else {
			while(!feof(iopipe) ) {
				if(fgets(psBuffer, 128, iopipe) != NULL) {
					cmd.response += string(psBuffer);
				}
			}
			_pclose(iopipe);
		}
	}

};

#endif // RUNNER_CPP
