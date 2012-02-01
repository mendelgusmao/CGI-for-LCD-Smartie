#include "stdafx.h"
#include "client.h"
#include "command.cpp"
#include "protocol.cpp"
#include "utils.cpp"

using boost::asio::ip::udp;
using boost::lexical_cast;
using boost::filesystem::exists;

string client::_app_path("");
string client::_scripts_path("");
string client::_ini_file("");
unsigned int client::_port(0);

void client::start() {
	_app_path = utils::app_path();
	_scripts_path = _app_path + "\\scripts";
	_ini_file = _scripts_path + "\\cgi4lcd.ini";
	_port = lexical_cast<unsigned int>(utils::ini_read(_ini_file, "cgi4lcd.port", "65432"));
}

string client::execute(string script, string parameters, bool version) {

	string buffer("");
	string arguments("");
	string extension("");
	string interpreter("");
	string execution_interval("");
	string execution_timeout("");
	map <string, string> vars;

	boost::filesystem::path p(script);
	extension = p.extension().string().substr(1);

	if (extension == "") {
		extension = utils::ini_read(_ini_file, "cgi4lcd.default", "");
		script += "." + extension;
	}

	interpreter = utils::ini_read(_ini_file, extension + ".interpreter", "");

	if (interpreter == "" || !exists(interpreter)) {
		return "[CGI4LCD] Interpreter for extension '" + extension + "' not found (" + interpreter + ")";
	}

	execution_interval = utils::ini_read(_ini_file, "cgi4lcd.interval", "15000");
	execution_timeout = utils::ini_read(_ini_file, "cgi4lcd.timeout", "30000");

	if (version) {
		arguments = utils::ini_read(_ini_file, extension + ".version", "");
	}
	else {
		arguments = utils::ini_read(_ini_file, extension + ".command", "");

		if (!exists(_scripts_path + "\\" + script)) {
			return "[CGI4LCD] Script '" + script + "' not found";
		}
	}

	vars["%interpreter%"] = interpreter;
	vars["%scripts_path%"] = _scripts_path;
	vars["%bootstraps_path%"] = _scripts_path + "\\bootstraps";
	vars["%script%"] = _scripts_path + "\\" + script;
	vars["%params%"] = parameters;
	vars["'"] = "\"";

	arguments = format_command(arguments, vars);
	interpreter = format_command(interpreter, vars);

	return request(interpreter, arguments, lexical_cast<unsigned int>(execution_interval), lexical_cast<unsigned int>(execution_timeout));
}

string client::request(string interpreter, string arguments, unsigned int interval, unsigned int timeout) {

	using boost::asio::ip::udp;

	command cmd;
	cmd.executable = interpreter;
	cmd.arguments = arguments;
	cmd.interval = interval;
	cmd.timeout = timeout;

	string buffer("");

	try	{

		boost::asio::io_service io_service;
		udp::endpoint receiver_endpoint = udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), lexical_cast<int>(_port));

		udp::socket socket(io_service);
		socket.open(udp::v4());

		string data = protocol::build(cmd);
		const char* send_buf = data.c_str();
		socket.send_to(boost::asio::buffer(send_buf, data.size()), receiver_endpoint);

		char recv_buf[1024]; 
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

		string received(recv_buf, len);

		buffer += received;

	}
	catch (std::exception& e) {
		buffer = string("[CGI4LCD] Error: ") + e.what() + " ";
	}

	return buffer;
}

string client::format_command(const string& command_template, const map<string, string> vars) {

	map<string, string>::const_iterator it;
	string formatted_command(command_template);

	for (it = vars.begin(); it != vars.end(); ++it) {
		formatted_command = boost::ireplace_all_copy(formatted_command, it->first, it->second);
	}

	return formatted_command;
}