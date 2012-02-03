#include "stdafx.h"
#include "client.h"
#include "utils.cpp"

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT  void 
__stdcall  SmartieInit()
{
    client::start();
}

extern "C" DLLEXPORT  void 
__stdcall  SmartieFini()
{}

extern "C" DLLEXPORT  int
__stdcall  GetMinRefreshInterval()
{
    return 1000; // 300 ms
}

extern "C" DLLEXPORT  char * 
__stdcall  function1(char *param1, char *param2)
{
    string script(param1);
    string parameters(param2);

    char buffer[1024];
    strcpy_s(buffer, client::execute(script, parameters).c_str());

    return buffer;
}

extern "C" DLLEXPORT  char * 
__stdcall  function2(char *param1, char *param2)
{
    string script(param1);
    string parameters(param2);

    char buffer[1024];
    strcpy_s(buffer, client::execute(script, parameters, true).c_str());

    return buffer;
}
    
extern "C" DLLEXPORT  char * 
__stdcall  function20(char *param1, char *param2)
{
    return "cgi.dll is part of CGI4LCD version 0.1 by MendelGusmao github.com/MendelGusmao";
}
    
