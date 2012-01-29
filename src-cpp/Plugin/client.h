#include "stdafx.h"

#ifndef CLIENT_H
#define CLIENT_H

using namespace std;

class client {

public:

	static void start(unsigned int remote_port, string ini_file, string scripts_path);
	static string execute(string script, string parameters, bool version=false);
	static string request(string interpreter, string arguments, unsigned int interval, unsigned int timeout);
	static string format_command(const string& command_template, const map<string, string> vars);

	static string _scripts_path;
	static string _ini_file;
	static unsigned int _remote_port;
};

#endif // CLIENT_CPP