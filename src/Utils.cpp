#include "stdafx.h"
#include "Utils.h"

string Utils::ini_read(const string ini_file, const string key, const string default_value) {

    try {

        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(ini_file, pt);
        string ini_value = pt.get<string>(key);

        return ini_value == "" ? default_value : ini_value;
    }
    catch (exception&) {
        return default_value;
    }
}

string Utils::app_path() {
    char path[2048];
    GetModuleFileNameA(NULL, path, 2048);
    boost::filesystem::path p(path);

    return p.parent_path().string();        
}

string Utils::run(const string &command_line, const string &short_line) {
    // Right usage of CreateProcess based on https://github.com/HeliumProject/Helium/blob/2fbbc93654e748c92f7ff9f607c966292fd5877a/Platform/Windows/Process.cpp

    HANDLE hReadPipe;
    HANDLE hWritePipe;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    string response("");

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        return "[CGI4LCD] Error running '" + short_line + "': Couldn't create pipes";
    }
    else {

        STARTUPINFO si;
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.wShowWindow = 0;
        si.hStdOutput = hWritePipe;
        si.hStdError = hWritePipe;

        PROCESS_INFORMATION pi;
        memset(&pi, 0, sizeof(pi));

        if (!CreateProcess(NULL, (LPSTR) command_line.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            ::CloseHandle(hReadPipe);
            ::CloseHandle(hWritePipe);
            response = "[CGI4LCD] Error running '" + short_line + "': Couldn't create process";
        }
        else {
            ::CloseHandle(hWritePipe);

            char buffer[80];
            unsigned long count;
            stringstream stream;
            BOOL success = TRUE;

            do {
                while (success = ReadFile(hReadPipe, buffer, sizeof(buffer), &count, NULL)) {
                    if (success) {
                        stream.write(buffer, count);
                    }
                    else {
                        if (::GetLastError() == ERROR_BROKEN_PIPE) {
                            response = "[CGI4LCD] Error running '" + short_line + "': Broken pipe";
                            break;
                        }
                        else {
                            return "[CGI4LCD] Unknown error running '" + short_line + "'";
                        }
                    }
                }
            } while(success && count);

            ::CloseHandle(hReadPipe);

            response = stream.str();

            DWORD result = 0;
            BOOL codeResult = ::GetExitCodeProcess(pi.hProcess, &result);

            ::CloseHandle(pi.hProcess);
            ::CloseHandle(pi.hThread);
        }
    }

    return response;
}