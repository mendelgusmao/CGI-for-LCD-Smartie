#include "stdafx.h"
#include "Worker.h"
#include "Utils.h"

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT  void 
__stdcall  SmartieInit()
{
    boost::thread worker_thread(boost::bind(&Worker::start, &Worker::instance()));
}

extern "C" DLLEXPORT  void 
__stdcall  SmartieFini()
{}

extern "C" DLLEXPORT  int
__stdcall  GetMinRefreshInterval()
{
    return Worker::instance()._refresh_interval;
}

// Places in the queue and execute
extern "C" DLLEXPORT  char * 
__stdcall  function1(char *param1, char *param2)
{
    string script(param1);
    string parameters(param2);

    char buffer[C4L_BUFFER_SIZE];
    strcpy_s(buffer, Worker::instance().execute(script, parameters).c_str());

    return buffer;
}

// Places in the queue without executing
extern "C" DLLEXPORT  char * 
__stdcall  function2(char *param1, char *param2)
{
    string script(param1);
    string parameters(param2);

    char buffer[C4L_BUFFER_SIZE];
    strcpy_s(buffer, Worker::instance().execute(script, parameters, false, false, false).c_str());

    return buffer;
}

// Executes without placing in the queue
extern "C" DLLEXPORT  char * 
__stdcall  function3(char *param1, char *param2)
{
    string script(param1);
    string parameters(param2);

    char buffer[C4L_BUFFER_SIZE];
    strcpy_s(buffer, Worker::instance().execute(script, parameters, false, true).c_str());

    return buffer;
}

// Retrieves interpreter version
extern "C" DLLEXPORT  char * 
__stdcall  function4(char *param1, char *param2)
{
    string script(param1);
    string parameters(param2);

    char buffer[C4L_BUFFER_SIZE];
    strcpy_s(buffer, Worker::instance().execute(script, parameters, true).c_str());

    return buffer;
}
    
extern "C" DLLEXPORT  char * 
__stdcall  function20(char *param1, char *param2)
{
    return "cgi.dll is part of CGI4LCD version 0.3 by MendelGusmao github.com/MendelGusmao";
}
    
