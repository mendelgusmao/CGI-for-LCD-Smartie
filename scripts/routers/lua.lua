function main()

    params = split(arg[1], "#")
	func_name = params[0]

    if func_name ~= "" then
    	func = _G[func_name]
    	if func then
    		print(func(split(params[1], ";")))
    	else
    		print("[CGI4LCD] Function '" .. func_name .. "' not found\n")
    	end
	end

end