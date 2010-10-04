Gem::Specification.new do |s|
  s.name         = "artemis-sandbox"
  s.version      = "0.0.1"
  s.platform     = Gem::Platform::CURRENT
  s.authors      = ["Jens Nockert"]
  s.email        = ["jens@nockert.se"]
  s.homepage     = "http://github.com/JensNockert/artemis-sandbox"
  s.summary      = "A wrapper for the OS X sandbox"
  s.description  = "A wrapper for the Mac OS X sandbox APIs, please use Dia instead if you want something secure"
  
  s.requirements << 'Mac OS X, 10.5 or later'
  
  s.extensions   << 'ext/extconf.rb'
  
  s.files        = Dir.glob("{ext,lib}/**/*") + %w(LICENSE README.markdown)
  
  s.require_path = ['lib']
end