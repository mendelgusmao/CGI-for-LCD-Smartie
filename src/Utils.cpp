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