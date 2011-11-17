from bootstraps.py import main

def hello():
    return "Hello, world!"

def say_hi(who):
    return "Hi, %s!" % who
    
def do_some_math(a, b, c):
    return "%s" % (int(a) * int(b) + int(c))
    
main()