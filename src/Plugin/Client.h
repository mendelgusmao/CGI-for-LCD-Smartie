#include "stdafx.h"

#ifndef CLIENT_H
#define CLIENT_H

using namespace std;

class Client {

public:

    string _app_path;
    string _scripts_path;
    string _ini_file;
    unsigned int _port;
    unsigned int _execution_interval;
    unsigned int _execution_timeout;
    int _refresh_interval;
    string _default_extension;

    void start();
    string execute(string script, const string &parameters, bool version=false, bool do_not_queue=false, bool add_and_run=true);
    string request(const string &interpreter, const string &arguments, unsigned int interval, unsigned int timeout, bool do_not_queue, bool add_and_run);
    string format_command(const string &command_template, const map<string, string> vars);

};

#endif // CLIENT_H