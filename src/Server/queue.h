#include "stdafx.h"
#include "command.h"

#ifndef QUEUE_H
#define QUEUE_H

using namespace std;

class queue {

public:

    queue(boost::asio::io_service& io_service);
    void add(command &cmd, bool add_and_run);
    void run();

    command get(string line);

private:

    boost::asio::deadline_timer _timer;
    map<string, command> _commands;
    
};

#endif // QUEUE_H
