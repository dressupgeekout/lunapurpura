#!/usr/bin/env ruby

$LOAD_PATH.unshift(File.join(__dir__, "..", "lib"))
require 'clu'

path = ARGV.shift

if not path
  $stderr.puts("expected a path")
  exit 1
end

path = File.expand_path(path)

$stderr.puts CLU.new(path).fancy_text
