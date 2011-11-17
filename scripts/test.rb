# $dll(cgi,1,test.rb,hello)
def hello
    "Hello, world!\n"
end

# $dll(cgi,1,test.rb,say_hi#Mendel)
def say_hi(who)
    "Hi, #{who}\n"
end

# $dll(cgi,1,test.rb,do_some_math#3;2;1)
def do_some_math(a, b, c)
    "%d\n" % (a.to_i * b.to_i + c.to_i)
end

main()