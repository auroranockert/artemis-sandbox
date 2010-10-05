module Artemis
  
  #
  # ProcessError, raised when a process cannot be started for some reason
  #
  
  class ProcessError < StandardError
  end
  
  #
  # Just to prepare Ruby
  #
  
  class Path
  end
  
  #
  # Artemis.sandbox(sandbox, command, *arguments)
  #   - sandbox
  #     * A Artemis::Sanbox instance
  #   - command
  #     * A string, representing the application to start
  #   - *arguments
  #     * Converted to strings and passed to the application in argv
  # Notes:
  #   Don't block exec in the .sb, or you won't be able to use this method.
  #   Pure computation for example, does not allow this, keep to Ruby if you
  #   need to block forking.
  #
  
  class << self
    def sandbox(sandbox, command, *arguments)
      Process.fork do
        case sandbox
        when Artemis::Sandbox
          sandbox.apply!
        else
          raise SecurityError, "invalid sandbox"
        end
        
        yield if block_given?
        
        exec(command.to_s, *arguments.collect { |arg| arg.to_s }) rescue nil
        
        raise ProcessError, "failed to execute #{command.to_s}#{ " #{arguments.join(' ')}" if arguments.length > 0 }"
      end
      
      Process.wait
      
      $?.success?
    end
    
    #
    # Artemis.system(profile, command, *arguments)
    #   - command
    #     * A string, representing the application to start
    #   - *arguments
    #     * Converted to strings and passed to the application in argv
    # Notes:
    #   Works like Artemis.sandbox, just without any limitations.
    #

    def system(command, *arguments)
      Process.fork do
        yield if block_given?
        
        exec(command.to_s, *args.collect { |arg| arguments.to_s }) rescue nil
        
        raise ProcessError, "failed to execute #{command.to_s}#{ " #{arguments.join(' ')}" if arguments.length > 0 }"
      end
      
      Process.wait
      
      $?.success?
    end
  end
end

require 'artemis_sandbox'