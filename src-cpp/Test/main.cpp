#include "stdafx.h"
#include "client.cpp"

int _tmain(int argc, _TCHAR* argv[])
{
	client::start();
	cout << "Response: '" << client::execute("test.php", "hello") << "'" << endl;
	cout << "Response: '" << client::execute("test.php", "say_hi#Mendel") << "'" << endl;
	cout << "Response: '" << client::execute("test.php", "do_some_math#10;20;30") << "'" << endl;

	string input("");
	getline(cin, input);
}

