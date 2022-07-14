#!/usr/bin/env ruby

require 'optparse'

$LOAD_PATH.unshift(File.join(__dir__, "..", "lib"))
require 'dib'

parser = OptionParser.new do |opts|
end
parser.parse!(ARGV)

path = ARGV.shift

if not path
  $stderr.puts("expected a path")
  exit 1
end

$stderr.puts DIB.new(File.expand_path(path)).fancy
