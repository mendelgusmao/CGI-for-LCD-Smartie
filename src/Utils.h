#include "stdafx.h"

#ifndef UTILS_H
#define UTILS_H

using namespace std;

class Utils {

public:

    static string ini_read(const string ini_file, const string key, const string default_value);
    static string app_path();
    static string run(const string &command_line, const string &short_line);

};

#endif // UTILS_H