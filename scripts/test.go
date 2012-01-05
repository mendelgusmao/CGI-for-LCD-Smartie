package main

import "strconv"
import "bootstrap"
import "strings"
import "os"

func hello() string {
  return "Hello, world!\n"
}

func say_hi(name string) string {
  return "Hi, " + name + "\n"
}

func do_some_math(a, b, c string) string {
  a_i, _ := strconv.Atoi(a)
  b_i, _ := strconv.Atoi(b) 
  c_i, _ := strconv.Atoi(c)
  
  return strconv.Itoa(a_i + b_i + c_i) + "\n"
}

func main() {
  functions := new(bootstrap.Functions)
  
  functions.Register("hello", hello)
  functions.Register("say_hi", say_hi)
  functions.Register("do_some_math", do_some_math)
  
  bootstrap.Main(functions)
}