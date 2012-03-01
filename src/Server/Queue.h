#include "stdafx.h"
#include "Command.h"

#ifndef QUEUE_H
#define QUEUE_H

using namespace std;

class Queue {

public:

    Queue(boost::asio::io_service& io_service);
    void add(Command &cmd);
    void run();
    Command get(const string& line);

private:

    boost::asio::deadline_timer _timer;
    map<string, Command> _commands;
    
};

#endif // QUEUE_H
