#include "stdafx.h"

#ifndef RUNNER_CPP
#define RUNNER_CPP

#include "command.cpp"

class runner {

public:
    int run(const command &cmd, const string dir) {

        string executable = cmd.executable;
        string arguments = cmd.arguments;

    }
};

#endif // RUNNER_CPP
