#include "stdafx.h"
#include "Client.h"

int _tmain(int argc, _TCHAR* argv[])
{
    Client::start();

    cout << "Response: '" << Client::execute("test.php", "hello") << "'" << endl;
    Sleep(1000);
    
    cout << "Response: '" << Client::execute("test.php", "say_hi#Mendel") << "'" << endl;
    Sleep(1000);
    
    cout << "Response: '" << Client::execute("test.php", "do_some_math#10;20;30") << "'" << endl;
    Sleep(1000);

    cout << "Response: '" << Client::execute("test.php", "not_implemented") << "'" << endl;
    Sleep(1000);        
    
    cout << "Response: '" << Client::execute("test", "hello") << "'" << endl;
    Sleep(1000);
    
    cout << "Response: '" << Client::execute("test", "say_hi#Mendel") << "'" << endl;
    Sleep(1000);
    
    cout << "Response: '" << Client::execute("test", "do_some_math#10;20;30") << "'" << endl;
    Sleep(1000);

    cout << "Response: '" << Client::execute("test", "not_implemented") << "'" << endl;
    Sleep(1000);

    int calls(0);
    double diff(0);
    double total_diff(0);

    while (true) {

        ++calls;

        time_t begin, end;

        time(&begin);
        cout << "(call " << calls << ") Response: '" << Client::execute("test", "do_some_math#10;20;30") << "' ";
        time(&end);

        diff = difftime(end, begin);
        total_diff += diff;

        cout << " in " << diff << " seconds (" << (total_diff / calls) << ")." << endl;

        Sleep(Client::_refresh_interval);
    }
    
    string input("");
    getline(cin, input);
}

