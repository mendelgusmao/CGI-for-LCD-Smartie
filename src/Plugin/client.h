#include "stdafx.h"

#ifndef CLIENT_H
#define CLIENT_H

using namespace std;

class client {

public:

    static void start();
    static string execute(string script, string parameters, bool version=false);
    static string request(string interpreter, string arguments, unsigned int interval, unsigned int timeout);
    static string format_command(const string& command_template, const map<string, string> vars);

    static string _app_path;
    static string _scripts_path;
    static string _ini_file;
    static unsigned int _port;
    static unsigned int _execution_interval;
    static unsigned int _execution_timeout;
    static string _default_extension;
};

#endif // CLIENT_CPP