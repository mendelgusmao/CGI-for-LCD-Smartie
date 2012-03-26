#include "stdafx.h"
#include "Command.h"

#ifndef CLIENT_H
#define CLIENT_H

using namespace std;

class Client {

public:

    string _app_path;
    string _scripts_path;
    string _ini_file;
    unsigned int _execution_interval;
    unsigned int _execution_timeout;
    int _refresh_interval;
    string _default_extension;
    unsigned int _max_threads;
    unsigned int _running_threads;
    map<string, Command> _commands;

    Client();
    void start();
    string execute(string script, const string &parameters, bool version=false, bool do_not_queue=false, bool add_and_run=true);
    string format_command(const string &command_template, const map<string, string> vars);
    void add(Command &command);
    void process();
    void run(Command &command);
    Command get(const string& line);

};

#endif // CLIENT_H