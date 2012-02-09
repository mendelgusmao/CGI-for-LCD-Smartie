#include "stdafx.h"
#include "utils.h"

#ifndef UTILS_CPP
#define UTILS_CPP

static string utils::ini_read(const string ini_file, const string key, const string default_value) {

    try {

        cout << "INI File: '" << ini_file << "'" << endl;

        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(ini_file, pt);
        string ini_value = pt.get<string>(key);

        if (ini_value == "") {
            return default_value;
        }
        else {

            if (ini_value.substr(0, 1) == "\"" && ini_value.substr(ini_value.size() - 1, 1) == "\"") {
                ini_value = ini_value.substr(1);
                ini_value = ini_value.substr(0, ini_value.size() - 1);
            }                

            return ini_value;
        }
    }
    catch (exception&) {
        return default_value;
    }
}

static string utils::app_path() {
    char path[2048];
    GetModuleFileNameA(NULL, path, 2048);
    boost::filesystem::path p(path);

    return p.parent_path().string();        
}

static wstring utils::s2ws(const string& s) { 
    int len; 
    int slength = (int)s.length() + 1; 
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);  
    wchar_t* buf = new wchar_t[len]; 
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len); 
    wstring r(buf); 
    delete[] buf; 
    return r; 
}