#include "stdafx.h"
#include "Worker.h"
#include "Command.h"
#include "Utils.h"

using boost::lexical_cast;
using boost::filesystem::exists;

Worker::Worker() :
    _app_path(Utils::app_path()),
    _scripts_path(_app_path + "\\scripts"),
    _ini_file(_scripts_path + "\\cgi4lcd.ini"),
    _execution_interval(lexical_cast<unsigned int>(Utils::ini_read(_ini_file, "cgi4lcd.interval", "15000"))),
    _execution_timeout(lexical_cast<unsigned int>(Utils::ini_read(_ini_file, "cgi4lcd.timeout", "30000"))),
    _refresh_interval(lexical_cast<int>(Utils::ini_read(_ini_file, "cgi4lcd.refresh", "1000"))),
    _default_extension(Utils::ini_read(_ini_file, "cgi4lcd.default_extension", "")),
    _max_threads(lexical_cast<unsigned int>(Utils::ini_read(_ini_file, "cgi4lcd.max_threads", "4"))),
    _running_threads(0)
{}

void Worker::start() {
    boost::asio::io_service io_service;
    boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(1));
    timer.async_wait(boost::bind(&Worker::process, this, boost::asio::placeholders::error, &timer));
    io_service.run();
}

string Worker::execute(string script, const string &parameters, bool version, bool do_not_queue, bool add_and_run) {

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

    Command command;
    command.executable = interpreter;
    command.arguments = arguments;
    command.interval = _execution_interval;
    command.timeout = _execution_timeout;
    command.do_not_queue = do_not_queue;
    command.add_and_run = add_and_run;

    add(command);

    return _commands[command.line()].response;
}

string Worker::format_command(const string &command_template, const map<string, string> vars) {

    map<string, string>::const_iterator it;
    string formatted_command(command_template);

    for (it = vars.begin(); it != vars.end(); ++it) {
        formatted_command = boost::ireplace_all_copy(formatted_command, it->first, it->second);
    }

    return formatted_command;
}

void Worker::add(Command &command) {

    map<string, Command>::iterator it = _commands.find(command.line());
    time_t now;
    time(&now);

    if (it == _commands.end()) {
        command.response = "";
        command.last_request = now;

        _commands[command.line()] = command;
    
        if (command.add_and_run) {
            run(command);
        }
    }
    else {
        _commands[command.line()].last_request = now;
    }
}

void Worker::process(const boost::system::error_code& /*e*/, boost::asio::deadline_timer *timer) {

    map<string, Command>::iterator it = _commands.begin();
    Command command;

    while (it != _commands.end()) {
        command = it->second;

        time_t now;
        time(&now);

        if (now >= command.last_request + command.timeout) {
            _commands.erase(it++);
            continue;
        }
        else if (_running_threads < _max_threads && command.is_running == false && now >= command.last_execution + command.interval) {
            boost::thread runner(boost::bind(&Worker::run, this, command));
        }

        _commands[command.line()] = command;
        ++it;
    }

    timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));
    timer->async_wait(boost::bind(&Worker::process, this, boost::asio::placeholders::error, timer));

}

void Worker::run(Command &command) {

    char buffer[128];
    FILE *iopipe;
    
    ++_running_threads;

    if (!command.do_not_queue) {
        _commands[command.line()].is_running = true;
    }

    iopipe = _popen(command.line().c_str(), "r");

    if (iopipe == NULL) {
        command.response = "[CGI4LCD] Error running...";
    }
    else {
        string response = "";

        while(!feof(iopipe)) {
            if(fgets(buffer, 128, iopipe) != NULL) {
                response += string(buffer);
            }
        }

        _pclose(iopipe);
        command.response = response;
    }

    map<string, Command>::iterator it = _commands.find(command.line());

    if (!command.do_not_queue) {

        if (it == _commands.end()) {
            _commands[command.line()] = command;
            time(&_commands[command.line()].last_request);
        }

        _commands[command.line()].is_running = false;
        _commands[command.line()].response = command.response;
        time(&_commands[command.line()].last_execution);
    }

    --_running_threads;
}

