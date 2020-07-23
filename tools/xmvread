#!/usr/bin/env ruby

soundfile_path = nil
imagesfile_path = nil
xmvfile_path = nil

require 'optparse'

parser = OptionParser.new do |opts|
  opts.on("--sound PATH") { |path| soundfile_path = File.expand_path(path) }
  opts.on("--images PATH") { |path| imagesfile_path = File.expand_path(path) }
end
parser.parse!(ARGV)

xmvfile_path = ARGV.shift

if not xmvfile_path
  $stderr.puts("expected a path to a XMV")
  exit 1
end

xmvfile_path = File.expand_path(xmvfile_path)

if not File.file?(xmvfile_path)
  $stderr.puts("no such file: #{xmvfile_path}")
  exit 1
end

xmvfile = File.open(xmvfile_path, "r")

MAGIC = [186, 126, 109, 3]

magic = xmvfile.read(4).unpack("C*")

if magic != MAGIC
  $stderr.puts("error: bad magic: got #{magic.inspect}, expected #{MAGIC.inspect}")
  exit 1
end

a = xmvfile.read(4).unpack("L>*")[0]
b = xmvfile.read(2).unpack("S>*")[0]
c = xmvfile.read(2).unpack("S>*")[0]
d = xmvfile.read(2).unpack("S>*")[0]
e = xmvfile.read(2).unpack("S>*")[0]

puts "a=#{a} b=#{b} c=#{c} d=#{d} e=#{e}"

type = xmvfile.read(4).unpack("L>*")[0]

puts "type=#{type}"

p xmvfile.read(12).unpack("S>*")

xmvfile.read(132) # zeroes

n_entries = xmvfile.read(4).unpack("L>*")[0]

marker = 0
i = 0

puts "TOTAL FRAMES = #{n_entries}"

until i >= n_entries
  marker = xmvfile.read(2).unpack("S>*")[0]
  xpkentry = xmvfile.read(2).unpack("S>*")[0]
  vals = xmvfile.read(12).unpack("S>*")
  print "frame=#{sprintf("%02d", i)}\t#{marker}\txpkentry=#{xpkentry}\t#{vals.inspect}"
  print "\n"
  i+=1
end

xmvfile.close
