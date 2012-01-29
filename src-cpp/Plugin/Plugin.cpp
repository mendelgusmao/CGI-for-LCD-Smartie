#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include "client.h"

#define DLLEXPORT __declspec(dllexport)



extern "C" DLLEXPORT  void 
__stdcall  SmartieInit()
{
	unsigned int port(0);
	string ini_file("");
	string scripts_path("");

	client::start(port, ini_file, scripts_path);
}

extern "C" DLLEXPORT  void 
__stdcall  SmartieFini()
{}

extern "C" DLLEXPORT  int
__stdcall  GetMinRefreshInterval()
{
	return 300; // 300 ms
}

extern "C" DLLEXPORT  char * 
__stdcall  function1(char *param1, char *param2)
{
    return "This is function 1";       
}

extern "C" DLLEXPORT  char * 
__stdcall  function2(char *param1, char *param2)
{
    static char outbuf[1000];
    static int count;
    
    count ++;
    
    _itoa_s(count, outbuf, 10);
    return outbuf;       
}
    
