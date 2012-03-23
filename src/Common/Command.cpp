#include "stdafx.h"
#include "Command.h"

using std::string;

Command::Command() {
    is_running = false;
    time(&last_execution);
    time(&last_request);
}

string Command::line() {
    return executable + " " + arguments;
}

string Command::shortline() {
    if (line().size() > 50) {
        return line().substr(line().size() - 40);
    }
    else {
        return line();
    }
}
