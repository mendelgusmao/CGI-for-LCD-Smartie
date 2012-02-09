#include "stdafx.h"

#ifndef UTILS_H
#define UTILS_H

using namespace std;

class utils {

public:

    static string utils::ini_read(const string ini_file, const string key, const string default_value);
    static string app_path();
    static std::wstring s2ws(const string& s);

};

#endif // UTILS_H