package router

import (
  "reflect"
  "os"
  "strings"
  "fmt"
)

type Functions map[string]reflect.Value

func (this Functions) Register(name string, function interface{}) {
  this[name] = reflect.ValueOf(function)
}

func (this Functions) Call(name string, parameters []string) string {
  return this[name](parameters)
}

func (this Functions) Exists(name string) (found bool) {
  _, found = this[name]
}

func Main(functions Functions) {
  
  params := strings.Split(os.Args[1], "#")
  function := params[0]
  
  if functions.Exists(function) {
    if len(params) == 1 {
      print(functions.Call(function, nil))
    } else {
      print(functions.Call(function, strings.Split(params[1], ";")))
    }
  } else {
    fmt.Printf("[CGI4LCD] Function '%s' not found\n", function)
  }
  
}
