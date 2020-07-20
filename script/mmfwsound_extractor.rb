#!/usr/bin/env ruby
#
# MMFW Sound Extractor
#
# This script uses mmfw(1) to determine the sample format of the raw audio
# stored in a MMA archive, and extracts each individual file to a more
# manageable format. Requires FFmpeg.
#

require 'optparse'
require 'shellwords'

mmfw_tool = nil

parser = OptionParser.new do |opts|
  opts.on("--mmfw PATH") { |path| mmfw_tool = File.expand_path(path) }
end
parser.parse!(ARGV)

if not mmfw_tool
  $stderr.puts(parser.to_s)
  exit 1
end

archive = ARGV.shift

if not archive
  $stderr.puts(parser.to_s)
  exit 1
end

archive = File.expand_path(archive)

if not File.file?(archive)
  $stderr.puts("no such archive: #{archive}")
  exit 1
end

def cmd(str)
  $stderr.puts("+ " + str)
  system(str)
end

IO.popen("#{Shellwords.escape(mmfw_tool)} -t #{Shellwords.escape(archive)}") do |stream|
  until stream.eof?
    line = stream.gets.chomp
    next if line =~ /mmfw:/

    elements = line.split("\t").map { |element| element.strip }
    id = elements[0]
    name = elements[1]
    fmt = elements[2]

    system %Q(#{mmfw_tool} -x -n #{id} #{archive} >/dev/null 2>&1)

    ffmpeg_common = "-y -v quiet"

    case fmt
    when "mono s16be @ 22050 Hz"
      cmd %Q(ffmpeg #{ffmpeg_common} -f s16be -ac 1 -ar 22050 -i #{name} #{name}.wav)
    when "mono s16be @ 44100 Hz"
      cmd %Q(ffmpeg #{ffmpeg_common} -f s16be -ac 1 -ar 44100 -i #{name} #{name}.wav)
    else
      $stderr.puts(">> don't know how to handle #{name} !")
      next
    end
  end
end
