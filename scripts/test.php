<?php

    # $dll(cgi,1,test.php,hello)
    function hello () {
        return "Hello, world!\n";
    }

    function say_hi ($name) {
	return "Hi, {$name}\n";
    }

    # $dll(cgi,1,test.php,do_some_math#3;2;1)
    function do_some_math ($a, $b, $c) {
        return ($a * $b + $c) . "\n";
    }

    main();

    echo "this can't be shown";

?>