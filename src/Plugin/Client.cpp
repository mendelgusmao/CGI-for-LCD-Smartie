#include "stdafx.h"
#include "Client.h"
#include "Command.h"
#include "Protocol.h"
#include "Utils.h"

using boost::asio::ip::udp;
using boost::lexical_cast;
using boost::filesystem::exists;

string Client::_app_path("");
string Client::_scripts_path("");
string Client::_ini_file("");
unsigned int Client::_port(0);
unsigned int Client::_execution_interval(0);
unsigned int Client::_execution_timeout(0);
int Client::_refresh_interval(0);
string Client::_default_extension("");
bool Client::_add_and_run(false);

void Client::start() {
    _app_path = Utils::app_path();
    _scripts_path = _app_path + "\\scripts";
    _ini_file = _scripts_path + "\\cgi4lcd.ini";
    _port = lexical_cast<unsigned int>(Utils::ini_read(_ini_file, "cgi4lcd.port", "65432"));
    _execution_interval = lexical_cast<unsigned int>(Utils::ini_read(_ini_file, "cgi4lcd.interval", "15000"));
    _execution_timeout = lexical_cast<unsigned int>(Utils::ini_read(_ini_file, "cgi4lcd.timeout", "30000"));
    _default_extension = Utils::ini_read(_ini_file, "cgi4lcd.default_extension", "");
    _refresh_interval = lexical_cast<int>(Utils::ini_read(_ini_file, "cgi4lcd.refresh", "1000"));
    _add_and_run = Utils::ini_read(_ini_file, "cgi4lcd.add_and_run", "1") == "1";
}

string Client::execute(string script, const string &parameters, bool version, bool do_not_queue) {

    string arguments("");
    string extension("");
    string interpreter("");
    map <string, string> vars;

    boost::filesystem::path p(script);
    extension = p.extension().string();

    if (version) {
        extension = script;

        if (extension == "") {
            extension = _default_extension;
        }
    }
    else if (extension == "") {
        extension = _default_extension;
        script += "." + extension;
    } 
    else {
        extension = extension.substr(1);
    }

    interpreter = Utils::ini_read(_ini_file, extension + ".interpreter", "");

    if (interpreter == "" || !exists(interpreter)) {
        return "[CGI4LCD] Interpreter for extension '" + extension + "' not found (" + interpreter + ")";
    }
    
    if (version) {
        arguments = Utils::ini_read(_ini_file, extension + ".version", "");
    }
    else {
        arguments = Utils::ini_read(_ini_file, extension + ".command", "");

        if (!exists(_scripts_path + "\\" + script)) {
            return "[CGI4LCD] Script '" + script + "' not found";
        }
    }

    script = boost::ireplace_all_copy(script, "/", "\\");

    vars["%interpreter%"] = interpreter;
    vars["%scripts_path%"] = _scripts_path;
    vars["%routers_path%"] = _scripts_path + "\\routers";
    vars["%router%"] = _scripts_path + "\\routers\\" + extension + "." + extension;
    vars["%script%"] = _scripts_path + "\\" + script;
    vars["%params%"] = parameters;
    vars["'"] = "\"";
    vars["´"] = "'";

    arguments = format_command(arguments, vars);
    interpreter = format_command(interpreter, vars);

    return request(interpreter, arguments, _execution_interval, _execution_timeout, do_not_queue);
}

string Client::request(const string &interpreter, const string &arguments, unsigned int interval, unsigned int timeout, bool do_not_queue) {

    using boost::asio::ip::udp;

    Command command;
    command.executable = interpreter;
    command.arguments = arguments;
    command.interval = interval;
    command.timeout = timeout;
    command.do_not_queue = do_not_queue;
    command.add_and_run = _add_and_run;

    string buffer("");

    try {

        boost::asio::io_service io_service;
        udp::endpoint receiver_endpoint = udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), lexical_cast<int>(_port));

        udp::socket socket(io_service);
        socket.open(udp::v4());

        string data = Protocol::build(command);
        const char* send_buf = data.c_str();
        socket.send_to(boost::asio::buffer(send_buf, data.size()), receiver_endpoint);

        char recv_buf[1024]; 
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        string received(recv_buf, len);

        buffer += received;

    }
    catch (boost::system::system_error s) {
        if (s.code().value() == 10054) {
            buffer = "[CGI4LCD] Error: server offline or listening on another port";
        }
        else {
            buffer = "[CGI4LCD] System Error: " + string(s.what()) + " ";
        }
    }
    catch (std::exception& e) {
        buffer = "[CGI4LCD] Error: " + string(e.what()) + " ";
    }

    return buffer;
}

string Client::format_command(const string &command_template, const map<string, string> vars) {

    map<string, string>::const_iterator it;
    string formatted_command(command_template);

    for (it = vars.begin(); it != vars.end(); ++it) {
        formatted_command = boost::ireplace_all_copy(formatted_command, it->first, it->second);
    }

    return formatted_command;
}