require("../routers/js.js");

// $dll(cgi,1,test.js,hello)
hello = function() {
  return "Hello, world!\n";
}

// $dll(cgi,1,test.js,say_hi#Mendel)
say_hi = function(name) {
  return "Hi, " + name + "\n";
}

// $dll(cgi,1,test.js,do_some_math#3;2;1)
do_some_math = function(a, b, c) {
  return (parseInt(a) * parseInt(b) + parseInt(c)) + "\n";
}

main(); 