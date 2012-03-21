#include "stdafx.h"
#include "Command.h"

#ifndef QUEUE_H
#define QUEUE_H

using namespace std;

class Queue {

public:

    Queue(boost::asio::io_service& io_service, unsigned int max_threads);
    void add(Command &cmd);
    void process();
    void run(Command &command);
    Command get(const string& line);

private:

    boost::asio::deadline_timer _timer;
    map<string, Command> _commands;
    unsigned int _max_threads;
    unsigned int _running_threads;
    
};

#endif // QUEUE_H
