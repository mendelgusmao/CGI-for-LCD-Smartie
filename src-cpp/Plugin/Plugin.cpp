// Plugin.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Plugin.h"


// This is an example of an exported variable
PLUGIN_API int nPlugin=0;

// This is an example of an exported function.
PLUGIN_API int fnPlugin(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Plugin.h for the class definition
CPlugin::CPlugin()
{
	return;
}
