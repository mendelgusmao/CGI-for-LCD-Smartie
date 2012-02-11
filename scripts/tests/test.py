from routers import main

# $dll(cgi,1,test.py,hello)
def hello():
    return "Hello, world!\n"

# $dll(cgi,1,test.py,say_hi#Mendel)
def say_hi(who):
    return "Hi, %s!\n" % who
    
# $dll(cgi,1,test.py,do_some_math#3;2;1)
def do_some_math(a, b, c):
    return "%s\n" % (int(a) * int(b) + int(c))
    
main()