#include "stdafx.h"

#ifndef COMMAND_H
#define COMMAND_H

using namespace std;

class Command {

public:

    string executable;
    string arguments;
    unsigned int timeout;
    unsigned int interval;
    time_t last_execution;
    time_t last_request;
    string response;
    bool is_malformed;
    bool is_internal;
    bool do_not_queue;
    bool add_and_run;
    bool is_running; 

    Command();
    string line();
    string shortline();
};

#endif // COMMAND_H



