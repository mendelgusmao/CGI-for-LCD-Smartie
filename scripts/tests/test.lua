-- $dll(cgi,1,test.lua,hello)
function hello()
    return("Hello, world!\n")
end

-- $dll(cgi,1,test.lua,say_hi#Mendel)
function say_hi(who)
    return("Hi, " .. who .. "\n")
end

-- $dll(cgi,1,test.lua,do_some_math#3;2;1)
function do_some_math(a, b, c)
	-- 3 * 2 + 1 
    return(a * b + c)
end

main()