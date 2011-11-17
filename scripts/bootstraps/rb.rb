def main()
  
  unless ARGV[0].nil?
  
    params = ARGV[0].split "#"

    if function = params[0].to_s
      if self.private_methods.include? function.to_sym
        puts send(function, *(params.size == 1 ? {} : params[1].to_s.split(";")))
      else
        puts "[CGI4LCD] Function '#{function}' not found"
      end
    end

  end  
    
  Kernel::exit
  
end