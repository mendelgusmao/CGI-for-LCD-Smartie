CGI FOR LCD SMARTIE 0.1-alpha
=============================

> This document describes the basic concept and usage of CGI for LCD Smartie.

> CGI for LCD Smartie is designed to be a fast gateway between LCD Smartie and language 
> interpreters such as PHP, Ruby and Python, making it easier to gather information to 
> show on the display without the need to create new plugins.

> The software is made from two parts: a server (cgi4lcd.exe) and a plugin (cgi.dll).  
> The plugin is responsible for sending the command to the server, receiving a response and relaying it to LCD Smartie.  
> The server receives the command, executes the specified interpreter, storing the command and its response in a queue. At a specified time interval it will run the command, caching its result. When the plugin asks the server for the response of a command, it will deliver the cached result instead of executing the interpreter before the specified time interval.

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
> * When CGI4LCD is dominating the market share of all other plugins and everyone starts developing scripts for fun and non-profit and someone created something useful to you

WHEN IT WILL NOT BE USEFUL?
---------------------------

> If you're happy and pleased using the other plugins  
> Basically, for everything that demands fast or realtime data displaying, like graph bars.  
> Believe me, the minimum time interval that LCD Smartie is allowed to get data from the plugin is 1 second and even if it was smaller, the default time interval to re-execute an interpretator is 15 seconds. Oh, are you thinking about decreasing everything to 100 ms? Only if you have a good machine configuration to support thousands of processes doing the same thing.  
> Really, don't try it. Go to C++

INSTALLING
----------

> * Make sure you have .Net Framework 4
> * Grab the latest version from [https://github.com/MendelGusmao/CGI-for-LCD-Smartie/tags](https://github.com/MendelGusmao/CGI-for-LCD-Smartie/tags)  
> * Copy plugins and scripts folder to your LCD Smartie folder

CONFIGURING
-----------

> The configuration is done in cgi4lcd.ini (avaliable in scripts folder).  

> The first section is [cgi4lcd]. It has the following attributes:

>   * interval (numeric, milliseconds)  
>     Value to tell the server of how often a command it will be executed

>   * timeout (numeric, milliseconds)  
>     Value to tell the server of how long after the command is not requested anymore by the plugin to be removed from the queue

>   * port (numeric, 1-65535)  
>     A numeric value representing the UDP port that will be used for the communication between server and plugin

>   * default (text)  
>     The default file extension to be considered by the plugin if not specified in the filename passed to $dll

>   * show_window (boolean, 0-1)  
>     If the value is 1, the server window will appear when it's executed, showing the queue processing and incoming requests

> The subsequent sections are named with the common file extension of the language. They have the following attributes:

>   * language (text)  
>     Language name for the extension. Not used right now

>   * interpreter (text)  
>     Absolute path for the language interpreter

>   * command  
>     The command line to execute the script. It has variables that are replaced when the plugin sends the command to the server. They are:

>       * %scripts_path%  
> 	The absolute path to *[LCD Smartie path]*\scripts folder

>       * %routers_path%  
> 	The absolute path to *[LCD Smartie path]*\scripts\routers folder

>       * %script%  
> 	The absolute path to *[LCD Smartie path]*\scripts\*[script name]*

>       * %params%  
> 	Function name or function name and parameters specified in the last argument passed to $dll

>       * %interpreter%  
> 	The path specified in the property interpreter

>   * version  
>     Command line to get the interpreter version

> CGI4LCD comes with the basic settings to run PHP, Ruby, Python, Perl and DOS Batch scripts. You must only change the interpreter path and it will be ready to use.

USING
-----

> There is only one working function in the plugin and it takes 2 parameters:  

> * filename - mandatory. it must exist in scripts folder.  
>   The extension is optional. If not passed, the plugin will use the default extension set in the section cgi4lcd of the configuration file  
> * function (optional) and arguments (separated by semicolons), also optional  

> Syntax: `$dll(cgi,1,filename[.extension],[function[#arg1;arg2;...]])`

> There are three possible usages for code executing:

> * Executing a PHP file

> Syntax: `$dll(cgi,1,file[.ext],)`

> * Executing a function of a PHP file without arguments

> Syntax: `$dll(cgi,1,file[.ext],function)`

> * Executing a function of a PHP file with arguments

> Syntax: `$dll(cgi,1,file[.ext],function#arg1;arg2;argX)`

WRITING SCRIPTS
---------------

> First of all, you'll need to know what is a router. It's a piece of code responsible for calling a function, retrieving its result and printing to the screen, so the server can grab it and relay it to the plugin. Routers are optional if your script directly outputs data but mandatory if you want to group functions in a file.

> For now, CGI4LCD comes with routers written in PHP, Ruby, Python and Perl. A Go version is in the repository - but still just a concept.

> The script must be saved in scripts folder and has the following structure:

> `# include router`  
> `# declare functions`  
> `main(); # Execute router`  

> or

> `[write code that outputs directly to the screen]`

> The scripts directory is full of test.*[extension]* files that describes the basics of writing functions. Try it yourself:

> `$dll(cgi,1,test,hello)` -> Hello, world!  
> `$dll(cgi,1,test,hi#Mendel)` -> Hi, Mendel  
> `$dll(cgi,1,test,do_some_math#1;2;3)` -> 5  

LANGUAGE SPECIFICS
------------------

> ### PHP
>   To give some boost in PHP process startup, I decided to include a php.ini in scripts directory. This is a stripped version that doesn't make PHP load too many extensions. However, if you need to load a specific extension, just uncomment the line, as you would do normally.

> ### Python
>   While PHP, Ruby and Perl allows to pass a file from the command line to be included in the called script, Python doesn't (or, at least, I don't know how to). When writing a Python script, put on the first line:  
>   `from routers import main`

ISSUES
------

> * It works only with LCD Smartie 5.4 (I mean not with the recent 5.4.2.92b)  
> * *[Your complaint here]*

TODO
----

> * Rewrite almost every function or method to use char* instead of strings
> * Make the plugin run cgi4lcd.exe if it's not being executed
> * Allow configuration of interval/timeout per-language, per-script and per-function
> * Use threads instead of a server process
> * *[Your suggestion here]*

FINALLY
-------

![LCD Smartie running the world](/MendelGusmao/CGI-for-LCD-Smartie/blob/master/pics/run-everything-in-the-world.jpg?raw=true "Beaultiful, isn't it?")