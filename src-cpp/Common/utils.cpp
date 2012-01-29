#include "stdafx.h"

#ifndef UTILS_CPP
#define UTILS_CPP

using namespace std;

class utils {

public:

	static string ini_read(const string ini_file, const string key, const string default_value) {

		try {
			boost::property_tree::ptree pt;
			boost::property_tree::ini_parser::read_ini(ini_file, pt);
			string ini_value = pt.get<string>(key);

			if (ini_value == "") {
				return default_value;
			}
			else {
				return ini_value;
			}
		}
		catch (exception&) {
			return default_value;
		}
	}

	static string app_path() {
		char path[2048];
		GetModuleFileNameA(NULL, path, 2048);

		string spath(path);

		return spath;
	}

	static string get_extension(const string filename) {
		size_t dot_pos = filename.find_last_of(".");
	
		if (dot_pos == string::npos) {
			return "";
		}

		return filename.substr(dot_pos + 1);
	}
};
#endif // UTILS_CPP