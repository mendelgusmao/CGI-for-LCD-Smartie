#include "stdafx.h"
#include "Client.h"
#include "Command.h"
#include "Protocol.h"
#include "Utils.h"

using boost::asio::ip::udp;
using boost::lexical_cast;
using boost::filesystem::exists;

/*
    string _app_path;
    string _scripts_path;
    string _ini_file;
    unsigned int _execution_interval;
    unsigned int _execution_timeout;
    int _refresh_interval;
    string _default_extension;
    unsigned int _max_threads;
    unsigned int _running_threads;
    boost::asio::io_service _io_service;
    boost::asio::deadline_timer _timer;
    map<string, Command> _commands;
*/

Client::Client() :
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

void Client::start() {
    boost::asio::io_service &io_service(boost::asio::io_service);
}

string Client::execute(string script, const string &parameters, bool version, bool do_not_queue, bool add_and_run) {

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
    vars["�"] = "'";

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

string Client::format_command(const string &command_template, const map<string, string> vars) {

    map<string, string>::const_iterator it;
    string formatted_command(command_template);

    for (it = vars.begin(); it != vars.end(); ++it) {
        formatted_command = boost::ireplace_all_copy(formatted_command, it->first, it->second);
    }

    return formatted_command;
}

void Client::add(Command &command) {

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

void Client::process() {

    map<string, Command>::iterator it = _commands.begin();
    Command command;

#ifdef DEBUG
    int queue_size = _commands.size();
    Utils::cls();
    echo("Running queue (" << queue_size << ")");

    string title(lexical_cast<string>(queue_size));
    title = "CGI4LCD - " + title + " command" + (queue_size > 1 ? "s" : "") + " in queue";
    SetConsoleTitle(Utils::s2ws(title).c_str());
#endif

    while (it != _commands.end()) {
        command = it->second;

        time_t now;
        time(&now);

#ifndef FULL_DEBUG
        echo("Command '" << command.line().substr(command.line().size() - 40) << "' === '" << command.response << "'");
#else
        echo("Command '" << command.line().substr(command.line().size() - 40) << "'");
        echo("Cleanup Time: " << command.last_request << " + " << command.timeout);
        echo("Next Execution: " << command.last_execution << " + " << command.interval);
        echo("Cached Response: '" << command.response << "'");
#endif

        if (now >= command.last_request + command.timeout) {
            echo("Erasing '" << command.shortline() << "'");

            _commands.erase(it++);
            continue;
        }
        else if (_running_threads < _max_threads && command.is_running == false && now >= command.last_execution + command.interval) {
            boost::thread runner(boost::bind(&Client::run, this, command));
        }

        _commands[command.line()] = command;
        ++it;
    }

    Sleep(1);
    process();

}

void Client::run(Command &command) {

    char psBuffer[128];
    FILE *iopipe;

#ifdef DEBUG_RUNS
    echo("Running '" << command.shortline() << "'");
#endif

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
            if(fgets(psBuffer, 128, iopipe) != NULL) {
                response += string(psBuffer);
            }
        }

        _pclose(iopipe);
        command.response = response;
    }

#ifdef DEBUG_RUNS
    echo("Runner response: '" << command.response << "'");
#endif

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
