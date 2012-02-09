#include "stdafx.h"
#include "client.cpp"

int _tmain(int argc, _TCHAR* argv[])
{
    client::start();

    cout << "Response: '" << client::execute("test.php", "hello") << "'" << endl;
    Sleep(1000);
    
    cout << "Response: '" << client::execute("test.php", "say_hi#Mendel") << "'" << endl;
    Sleep(1000);
    
    cout << "Response: '" << client::execute("test.php", "do_some_math#10;20;30") << "'" << endl;
    Sleep(1000);

    cout << "Response: '" << client::execute("test.php", "not_implemented") << "'" << endl;
    Sleep(1000);        
    
    cout << "Response: '" << client::execute("test", "hello") << "'" << endl;
    Sleep(1000);
    
    cout << "Response: '" << client::execute("test", "say_hi#Mendel") << "'" << endl;
    Sleep(1000);
    
    cout << "Response: '" << client::execute("test", "do_some_math#10;20;30") << "'" << endl;
    Sleep(1000);

    cout << "Response: '" << client::execute("test", "not_implemented") << "'" << endl;
    Sleep(1000);

    int calls(0);
    double diff(0);
    double total_diff(0);

    while (true) {

        ++calls;

        time_t begin, end;

        time(&begin);
        cout << "(call " << calls << ") Response: '" << client::execute("test", "do_some_math#10;20;30") << "' ";
        time(&end);

        diff = difftime(end, begin);
        total_diff += diff;

        cout << " in " << diff << " seconds (" << (total_diff / calls) << ")." << endl;

        Sleep(client::_refresh_interval);
    }
    
    string input("");
    getline(cin, input);
}

