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
	_app_path = utils::app_path() + "\\..\\";
	_scripts_path = _app_path + "\\scripts\\";
	_ini_file = _scripts_path + "cgi4lcd.ini";
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

	extension = utils::get_extension(script);

	if (extension == "") {
		extension = utils::ini_read(_ini_file, "cgi4lcd.default", "");
		script += "." + extension;
	}

	interpreter = utils::ini_read(_ini_file, extension + ".interpreter", "");

	if (interpreter == "" || !exists(interpreter)) {
		return "[CGI4LCD] Interpreter for extension '" + extension + "' not found";
	}

	execution_interval = utils::ini_read(_ini_file, "cgi4lcd.interval", "15");
	execution_timeout = utils::ini_read(_ini_file, "cgi4lcd.timeout", "30");

	if (version) {
		arguments = utils::ini_read(_ini_file, extension + ".version", "");
	}
	else {
		arguments = utils::ini_read(_ini_file, extension + ".command", "");

		if (!exists(_scripts_path + script)) {
			return "[CGI4LCD] Script '" + script + "' not found";
		}
	}

	vars["interpreter"] = interpreter;
	vars["scripts_path"] = _app_path;
	vars["bootstraps_path"] = _app_path + "scripts\\bootstraps\\";
	vars["script"] = script;
	vars["arguments"] = arguments;
	vars["'"] = "\"";

	arguments = format_command(arguments, vars);

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

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "127.0.0.1", lexical_cast<string>(_port));
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		udp::socket socket(io_service);
		socket.open(udp::v4());

		const char* send_buf = protocol::build(cmd).c_str();
		socket.send_to(boost::asio::buffer(send_buf, strlen(send_buf)), receiver_endpoint);

		boost::array<char, 512> recv_buf;
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(
		boost::asio::buffer(recv_buf), sender_endpoint);

		buffer = string(recv_buf.data());

	}
	catch (std::exception& e) {
		buffer = string("[CGI4LCD] Error: ") + e.what();
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