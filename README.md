CGI FOR LCD SMARTIE 0.2-alpha
=============================

> CGI for LCD Smartie is designed to be a fast gateway between LCD Smartie ([lcdsmartie.sourceforge.net](http://lcdsmartie.sourceforge.net)) and language 
> interpreters such as PHP, Ruby or Python, making it easier to gather information to 
> show on the display without the need to create new plugins.

> The software is made from two parts: a server (cgi4lcd.exe) and a plugin (cgi.dll).  
> The plugin is responsible for sending the command to the server, receiving a response and relaying it to LCD Smartie.  
> The server receives the command, executes the specified interpreter, storing the command and its response in a queue. At a specified time interval it will execute the command, caching its result. When the plugin asks the server for the response of a command, it will deliver the cached result instead of executing the interpreter again. Also, there is a timeout value to tell the server when to remove a command if it is not being requested anymore.

WHAT HAPPENED TO PHP FOR LCD SMARTIE?
---------------------------------------------------

> First, I wanted to not to limit the plugin to run only PHP code.

> In the first version, the plugin was responsible for the execution of the interpreter.
> Operating in this way led to a problem: LCD Smartie was freezing from the beginning to the end of the execution of the script. I took the opportunity of trying other languages to make a radical change in plugin's architecture. Using polling, caching and a fast protocol like UDP solved the problem at the cost of not having quick new data but quick responses to display.

WHEN IT WILL BE USEFUL?
-----------------------

> * When you want to show data gathered from databases or internet applications (mysql reports, twitter, games)
> * When you want to develop that in a short timespan, using your preferred programming language
> * When you don't have patience to build complex plugins using C++, Delphi or VB
> * When you are really not interested in worrying about pointers, variable types, compilation and other cumbersome constraints that strongly typed languages has to bother you
> * When the time comes and CGI4LCD is dominating the market share of all other plugins so everyone starts developing scripts for fun and non-profit and someone creates something useful to you

WHEN IT WILL NOT BE USEFUL?
---------------------------

> If you're happy and pleased using the other plugins  
> Basically, for everything that demands fast or realtime data displaying, like graph bars.  
> Believe me, the minimum time interval that LCD Smartie is allowed to get data from the plugin is 1 second and even if it was smaller, the default time interval to re-execute an interpreter is 15 seconds. Oh, are you thinking about decreasing everything to 100 ms? Only if you have a good machine configuration to support thousands of processes doing the same thing.  
> Really, don't try it. I beg you to use C++

INSTALLING
----------

> * Make sure you have .Net Framework 4
> * Make sure you have LCD Smartie 5.4 or 5.4.2.92b (or grab it from [lcdsmartie.sourceforge.net](http://lcdsmartie.sourceforge.net/smartied.htm))  
> * Grab the latest version from [downloads](https://github.com/MendelGusmao/CGI-for-LCD-Smartie/downloads)  
> * Copy plugins and scripts directory to your LCD Smartie directory
> * Boost >= 1.48 ([boost.org](http://boost.org)) and UPX ([upx.sourceforge.net](http://upx.sourceforge.net)) if you want to have some fun compiling the project by yourself

CONFIGURING
-----------

> The configuration is done in cgi4lcd.ini (available in scripts directory).  

> The first section is [cgi4lcd]. It has the following attributes:

>   * interval (numeric, seconds)  
>     Value to tell the server of how often a command it will be executed

>   * timeout (numeric, seconds)  
>     Value to tell the server of how long after the command is not requested anymore by the plugin to be removed from the queue  

>   * refresh (numeric, seconds)  
>     Value to tell LCD Smartie how often it will request data from plugin
  
>   * port (numeric, 1-65535)  
>     A numeric value representing the UDP port that will be used for the communication between server and plugin

>   * default_extension (text)  
>     The default file extension to be considered by the plugin if not specified in the filename passed to $dll

>   * max_threads (numeric)  
>     The maximum number of commands that can be run simultaneously

> The subsequent sections are named with the common file extension of the language. They have the following attributes:

>   * language (text)  
>     Language name relative to the extension. Not used right now

>   * interpreter (text)  
>     Absolute path to the language interpreter

>   * command (text)  
>     The command line to execute the script. It has variables that are replaced when the plugin sends the command to the server. They are:

>       * %scripts_path%  
> 	The absolute path to *[LCD Smartie path]*\scripts directory

>       * %routers_path%  
> 	The absolute path to *[LCD Smartie path]*\scripts\routers directory

>       * %script%  
> 	The absolute path to *[LCD Smartie path]*\scripts\*[script name]*

>       * %params%  
> 	Function name or function name and parameters specified in the last argument passed to $dll

>       * %interpreter%  
> 	The path specified in the property interpreter

>       * Apostrophes will be replaced by quotes (INI limitation) and acute accents (´) will be replaced by apostrophes  

>   * version  
>     Command line to get the interpreter version

> CGI4LCD comes with the basic settings to run PHP, Ruby, Python, Lua and Perl scripts. You must only change the interpreter path and it will be ready to use.

USING
-----

> ### function1 - Executes interpreter  

> * filename - mandatory. it must exist in scripts directory.  
>   The extension is optional. If not passed, the plugin will use the default extension set in the section *[cgi4lcd]* of the configuration file  
> * function (optional) and arguments (separated by semicolons), also optional  

> Syntax: `$dll(cgi,1,filename[.extension],[function[#arg1;arg2;...]])`

> There are three possible usages for code executing:

> * Executing a script

> Syntax: `$dll(cgi,1,file[.ext],)`

> * Executing a function of a script without arguments

> Syntax: `$dll(cgi,1,file[.ext],function)`

> * Executing a function of a script with arguments

> Syntax: `$dll(cgi,1,file[.ext],function#arg1;arg2;argX)`

> ### function2 - Places the command in the queue without executing it
> * The same arguments of function1  

> ### function3 - Executes the command without placing it in a queue  
> * The same arguments of function1  

> ### function4 - Gathers interpreter version by extension
> * extension - optional. If empty, will use the default extension  

> Syntax: `$dll(cgi,3,[extension],)`  

> ### function20 - Credits
> * No arguments required

WRITING SCRIPTS
---------------

> First of all, you'll need to know what is a router: a piece of code responsible for calling a function, retrieving its result and printing to the screen, so the server can grab and relay it to the plugin. Routers are optional if your script outputs data directly to screen but mandatory if you want to group functions in a file.

> The script must be saved in scripts directory and has the following structure:

> `# include router`  
> `# declare functions`  
> `main(); # Execute router`  

> or

> `[write code that outputs directly to the screen]`

> The *scripts/tests* directory is full of test.*[extension]* files that describes the basics of writing functions. Try it yourself:

> `$dll(cgi,1,test,hello)` -> Hello, world!  
> `$dll(cgi,1,test,hi#Mendel)` -> Hi, Mendel  
> `$dll(cgi,1,test,do_some_math#1;2;3)` -> 5  

BUNDLED LANGUAGES 
-----------------

> *When I say "bundled", I mean it already has routers and basic configuration for the mentioned languages. You still need to download the interpreters from the internet to use their languages*

> ### PHP · PYTHON · RUBY · PERL · LUA · JAVASCRIPT  

> Or... Write your own. I'll be glad to add your contribution to the project. 

LANGUAGE SPECIFICS
------------------

> ### PHP
>   To give some boost to PHP interpreter startup, write a crafted php.ini. This must a stripped version that doesn't make PHP load too many extensions. Add the -c parameter to command template in *[php]* section. e.g.:  

> `command="-d auto_prepend_file='%routers_path%\php.php' -c '%scripts_path%\php.ini' '%script%' '%params%'"`

> ### Python
>   While PHP (e.g.) allows to pass a file from the command line to be included in the called script, Python doesn't (or, at least, I don't know how to). When writing a Python script, put on the first line:  

>   `from routers import main`

> ### Lua
>   The interpreter can't load a module by its full path using the "-l" command line parameter. Copy **[*LCD Smartie installation dir*]\scripts\router\lua.lua** to *[Lua installation dir]*\lua before using CGI4LCD with Lua.

> ### Perl
>   The interpreter can't load a module by its full path. Instead, the path to routers directory is passed using "-I" parameter. When writing a Perl script, put on the first line:  

>   `require("pl.pl")`  

> ### JavaScript (using [node.js](http://nodejs.org))
>   The interpreter doesn't provide a way to specify a path or a module to be loaded before the script to be run, so, put on the first line of your script:  

>   `require("routers/js.js")`  

ISSUES
------

> * It **will** halt LCD Smartie if the server is shut down while the client is requesting data
> * *[Your complaint here]*

TODO
----

> * Provide better support to Unicode strings
> * Rewrite almost every function or method to use char* instead of strings
> * Make the plugin run cgi4lcd.exe if it's not being executed
> * In the future, drop the server-client architecture and make the plugin use threads to do the server work
> * Allow configuration of interval/timeout per-language, per-script and per-function
> * *[Your suggestion here]*

FINALLY
-------

> * Official website of LCD Smartie: [lcdsmartie.org](http://lcdsmartie.org)
> * Official CGI4LCD thread at LCD Smartie forums: [forums.lcdsmartie.org](http://forums.lcdsmartie.org/viewtopic.php?f=8&t=3274)
> * Contact the author: [mendelsongusmao+cgi4lcd@gmail.com](mailto:mendelsongusmao+cgi4lcd@gmail.com)
