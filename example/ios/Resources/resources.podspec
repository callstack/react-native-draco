Pod::Spec.new do |s|
  s.name             = 'Resources'
  s.version          = '0.0.1-dev'
  s.author           = { 'Oskar Kwaśniewski' => 'oskarkwasniewski@icloud.com' }
  s.license          = 'MIT'
  s.homepage         = 'https://github.com/callstack/react-native-draco'
  s.source           = { :git => 'https://github.com/callstack/react-native-draco' }
  s.summary          = 'Resources for AwesomeApp'
  s.resources = '*.{drc}'
end
