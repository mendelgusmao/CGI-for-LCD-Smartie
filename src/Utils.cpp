#include "stdafx.h"
#include "Utils.h"

string Utils::ini_read(const string ini_file, const string key, const string default_value) {

    try {

        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(ini_file, pt);
        string ini_value = pt.get<string>(key);

        if (ini_value == "") {
            return default_value;
        }
        else {

            if (ini_value.substr(0, 1) == "\"" && ini_value.substr(ini_value.size() - 1, 1) == "\"") {
                ini_value = ini_value.substr(1, ini_value.size() - 2);
            }                

            return ini_value;
        }
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

wstring Utils::s2ws(const string& s) { 
    int len; 
    int slength = (int)s.length() + 1; 
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);  
    wchar_t* buf = new wchar_t[len]; 
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len); 
    wstring r(buf); 
    delete[] buf; 
    return r; 
}

void Utils::cls() {
    
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

   COORD coordScreen = { 0, 0 };    // home for the cursor 
   DWORD cCharsWritten;
   CONSOLE_SCREEN_BUFFER_INFO csbi; 
   DWORD dwConSize;

    // Get the number of character cells in the current buffer. 

   if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
      return;
   dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

   // Fill the entire screen with blanks.

   if( !FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
      dwConSize, coordScreen, &cCharsWritten ))
      return;

   // Get the current text attribute.

   if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
      return;

   // Set the buffer's attributes accordingly.

   if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
      dwConSize, coordScreen, &cCharsWritten ))
      return;

   // Put the cursor at its home coordinates.

   SetConsoleCursorPosition( hConsole, coordScreen );
}