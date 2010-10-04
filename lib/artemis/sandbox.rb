module Artemis
  
  #
  # ProcessError, raised when a process cannot be started for some reason
  #
  
  class ProcessError < StandardError
  end
  
  #
  # SandboxError, raised when there was a problem dropping privileges
  #
  
  class SandboxError < StandardError
  end
  
  #
  # Just to prepare Ruby
  #
  
  class Path
  end
  
  #
  # Artemis.sandbox(profile, command, *arguments)
  #   - profile
  #     * :no_internet
  #     * :no_network
  #     * :no_filesystem_write
  #     * :no_filesystem_write_outside_tmp
  #     * :pure_computation
  #     * An absolute path to a .sb file, as a String or Artemis::Path
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
    def sandbox(profile, command, *arguments)
      Process.fork do
        case profile
        when Symbol
          Artemis::Sandbox.predefined(profile)
        when String, Artemis::Path
          Artemis::Sandbox.config(profile.to_s)
        else
          raise SandboxError, "invalid profile type, need a path or symbol"
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