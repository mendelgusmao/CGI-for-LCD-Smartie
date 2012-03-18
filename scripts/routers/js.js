main = function() {

  if (typeof process.argv[2] === "undefined") return;

  params = process.argv[2].split("#");
  
  if (function_name = params[0]) {
    if (eval("typeof " + function_name) === "function") {
      func = eval(function_name);
      console.log(func.apply(func, 1 == params.length ? [] : params[1].split(";")));
    }
    else {
      console.log("[CGI4LCD] Function " + function_name + " not found\n");
    }
  }
}