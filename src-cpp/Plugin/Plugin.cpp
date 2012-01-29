#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
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

extern "C" DLLEXPORT  const char * 
__stdcall  function1(char *param1, char *param2)
{
	string script(param1);
	string parameters(param2);

	char* response = const_cast<char*>(client::execute(script, parameters).c_str());
	return response;
}

extern "C" DLLEXPORT  char * 
__stdcall  function2(char *param1, char *param2)
{
	string script(param1);
	string parameters(param2);

	char* response = const_cast<char*>(client::execute(script, parameters, true).c_str());
	return response;
}
    
