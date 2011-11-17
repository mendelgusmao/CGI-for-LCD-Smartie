def main
  
  unless ARGV[0].nil?
  
    params = ARGV[0].split "#"

    if function = params[0]
      # Not working - how to check if the function exists?
      if Kernel.respond_to? function.to_s
        puts send(function, *(params.size == 1 ? {} : params[1].to_s.split(";")))
      else
        puts "[CGI4LCD] Function '" + function + "' not found"
      end
    end

  end  
    
  Kernel::exit
  
end