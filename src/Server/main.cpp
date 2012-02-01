#include "stdafx.h"
#include "server.cpp"
#include "utils.cpp"

int main(int argc, char* argv[]) {

	string ini_file(utils::app_path() + "\\..\\scripts\\cgi4lcd.ini");
	unsigned int port = lexical_cast<unsigned int>(utils::ini_read(ini_file, "cgi4lcd.port", "65432"));
	bool show_window = utils::ini_read(ini_file, "cgi4lcd.show_window", "0") == "1";

	if (!show_window) {
		HWND hWnd = GetConsoleWindow();
		ShowWindow( hWnd, SW_HIDE );
	}

    try {
        boost::asio::io_service io_service;
        server s(io_service, port);
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}