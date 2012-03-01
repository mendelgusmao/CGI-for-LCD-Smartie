#include "stdafx.h"

#ifndef CLIENT_H
#define CLIENT_H

using namespace std;

class Client {

public:

    static string _app_path;
    static string _scripts_path;
    static string _ini_file;
    static unsigned int _port;
    static unsigned int _execution_interval;
    static unsigned int _execution_timeout;
    static int _refresh_interval;
    static string _default_extension;
    static bool _add_and_run;

    static void start();
    static string execute(string script, const string &parameters, bool version=false, bool do_not_queue=false);
    static string request(const string &interpreter, const string &arguments, unsigned int interval, unsigned int timeout, bool do_not_queue);
    static string format_command(const string &command_template, const map<string, string> vars);

};

#endif // CLIENT_H