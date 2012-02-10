#include "stdafx.h"
#include "Command.h"

using std::string;

Command::Command() :
    timer(0),
    cleanup_timer(0)
{}

string Command::line() {
    return executable + " " + arguments;
}

void Command::run() {

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
