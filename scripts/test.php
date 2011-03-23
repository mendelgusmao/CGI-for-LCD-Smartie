<?php

    require "main.php";

    # $dll(php,1,test,hello)
    function hello () {
        echo "Hello, world!";
    }

    # $dll(php,1,test,do_some_math#3;2;1)
    function do_some_math ($param1, $param2, $param3) {
        echo $param1 * $param2 + $param3;
    }

    main();

    echo "-> normal execution";

?>
