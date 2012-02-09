#include "command.h"

using std::string;

command::command() :
    timer(0),
    cleanup_timer(0)
{}

string command::line() {
    return executable + " " + arguments;
}

void command::run() {

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
