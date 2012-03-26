#include "stdafx.h"

#ifndef UTILS_H
#define UTILS_H

using namespace std;

class Utils {

public:

    static string ini_read(const string ini_file, const string key, const string default_value);
    static string app_path();
    static wstring s2ws(const string& s);
    static void cls();

};

#endif // UTILS_H