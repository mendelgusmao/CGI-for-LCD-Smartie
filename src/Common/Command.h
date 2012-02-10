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
    unsigned int timer;
    unsigned int cleanup_timer;
    string response;
    bool is_malformed;
    bool is_internal;
    bool do_not_queue;

    Command();
    string line();
    void run();
};

#endif // COMMAND_H



