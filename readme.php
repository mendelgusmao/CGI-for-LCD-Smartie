<?php 

    error_reporting (E_ERROR | E_PARSE);

    function main () {

        global $argv;

        $params = explode("#", $argv[1]);

        if ($function = $params[0])
            if (function_exists($function))
                die(
                    call_user_func_array(
                        $function,
                        1 == count($params)
                           ? array()
                           : explode(";", $params[1])
                    )
                );
            else
                die("[php] function '{$params[0]}' doesn't exists\n");

    }

    # $dll(php,1,readme,helloworld)
    function helloworld () {
        echo "hello, world!";
    }

    # $dll(php,1,readme,puts#foo;bar;baz)
    function puts ($param1, $param2, $param3) {
        echo "$param1 + $param2 = $param3\n";
    }

    main();

    echo "\n-> normal execution\n";
?>
