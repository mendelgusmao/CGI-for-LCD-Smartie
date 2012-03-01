#include "stdafx.h"
#include "Server.h"
#include "Utils.h"

using boost::lexical_cast;

int main(int argc, char* argv[]) {

    string ini_file(Utils::app_path() + "\\..\\scripts\\cgi4lcd.ini");
    unsigned int port = lexical_cast<unsigned int>(Utils::ini_read(ini_file, "cgi4lcd.port", "65432"));

#ifndef DEBUG
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
#endif

    try {
        boost::asio::io_service io_service;
        Server s(io_service, port);
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}