require "pl.pl";

# $dll(cgi,1,test.php,hello)
sub hello {
    "Hello, world!\n";
}

# $dll(cgi,1,test.php,say_hi#Mendel)
sub say_hi {
    "Hi, $_[0]\n";
}

# $dll(cgi,1,test.php,do_some_math#3;2;1)
sub do_some_math {
    $_[0] * $_[1] + $_[2] . "\n";
}

main();
