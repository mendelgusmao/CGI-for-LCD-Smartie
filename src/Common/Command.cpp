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

