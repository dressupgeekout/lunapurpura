#!/usr/bin/env ruby
#
# unpack_entire_game.rb
#
# This script accepts a path to the RESOURCE/ directory of a Purple Moon
# game, and extracts every PRX (or PRD/PRS) archive.
#
# This file is part of Luna Purpura.
#

require 'fileutils'
require 'optparse'
require 'pathname'
require 'shellwords'

def cmd(str)
  $stderr.puts("+ " + str)
  system(str)
end

def h(str)
  return Shellwords.escape(str)
end

PROGNAME = File.basename($0)

resource_dir = nil
out_dir = Pathname.new(".").expand_path
prxtool = "prx"
expect_prdprs = false

parser = OptionParser.new do |opts|
  opts.banner = "usage: #{PROGNAME} [options ...] <RESOURCE-DIR>"
  opts.on("-o", "--outdir PATH") { |path| out_dir = File.expand_path(path) }
  opts.on("--prx PATH", "path to prx(1) tool") { |path| prxtool = File.expand_path(path) }
  opts.on("--new-school", "This is Rockett's New School data") { expect_prdprs = true }
end

parser.parse!(ARGV)

resource_dir = ARGV.shift

if not resource_dir
  $stderr.puts("expected a path to a game's RESOURCE directory")
  $stderr.puts(parser.to_s)
  exit 1
end

resource_dir = File.expand_path(resource_dir)

if not File.directory?(resource_dir)
  $stderr.puts "no such directory: #{resource_dir}"
  exit 1
end

if expect_prdprs
  Dir.glob("#{resource_dir}/**/*.PRD").each do |prd|
    prs = prd.sub(/\.PRD$/, ".PRS")
    new_dir = File.join(out_dir, Pathname.new(prd).relative_path_from(Pathname.new(resource_dir)).sub(/\.PRD$/, ""))
    FileUtils.mkdir_p(new_dir)
    cmd %Q(cd #{h(new_dir)} && #{h(prxtool)} -x -a #{h(File.realpath(prd))} #{h(File.realpath(prs))})
  end
else
  Dir.glob("#{resource_dir}/**/*.PRX").each do |prx|
    new_dir = File.join(out_dir, Pathname.new(prx).relative_path_from(Pathname.new(resource_dir)).sub(/\.PRX$/, ""))
    FileUtils.mkdir_p(new_dir)
    cmd %Q(cd #{h(new_dir)} && #{h(prxtool)} -x -a #{h(File.realpath(prx))})
  end
end
