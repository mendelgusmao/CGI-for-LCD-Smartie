function main()

    split = function(str, sep)
        arr = {}
        j = 1

        if str == nil then str = "" end

        for i in str:gmatch("[^" .. sep .. "]+") do
            arr[j] = i
            j = j + 1
        end

        return arr
    end

    local params = split(arg[1], "#")
	local func_name = params[1]

    if func_name ~= "" then
    	local func = _G[func_name]
    	if type(func) == "function" then
    		print(func(unpack(split(params[2], ";"))))
    	else
    		print("[CGI4LCD] Function '" .. func_name .. "' not found\n")
    	end
	end

end