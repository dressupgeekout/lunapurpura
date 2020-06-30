#!/usr/bin/env ruby
#
# unpack_entire_game.rb
#
# This script accepts a path to the RESOURCE/ directory of a Purple Moon
# game, and extracts every PRX (or PRD/PRS) archive.
#
# This file is part of Luna Purpura.
#

require 'pathname'

resource_dir = ARGV.shift

if not resource_dir
  $stderr.puts "expected a path to a game's RESOURCE directory" 
  exit 1
end

resource_dir = Pathname.new(resource_dir)

if not resource_dir.directory?
  $stderr.puts "no such directory: #{resource_dir}"
  exit 1
end

Pathname.glob("#{resource_dir}/**/*.PRX").each do |prx|
  new_dir = resource_dir.join(prx.relative_path_from(resource_dir).sub(/\.PRX$/, ""))
  puts prx
  new_dir.mkdir if not new_dir.directory?
  system %Q(cd #{new_dir} && #{__dir__}/../build/src/prx/prx -x -a #{prx.realpath})
end
