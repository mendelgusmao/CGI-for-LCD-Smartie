<?php

    function main () {

        global $argv;

        $params = explode("#", $argv[1]);

        if ($function = $params[0])
            die(
                function_exists($function)?
                    call_user_func_array(
                        $function,
                        1 == count($params)?
                          array()
                          : explode(";", $params[1])
                    )
                    : "[CGI4LCD] Function '{$function}' not found\n"
            );
    }

?>