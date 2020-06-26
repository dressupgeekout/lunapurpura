#!/usr/bin/env ruby

require 'optparse'

$LOAD_PATH.unshift File.join(__dir__, "..", "lib")
require 'clu'

clufile = nil
xpkfile = nil

parser = OptionParser.new do |opts|
  opts.on("-c", "--clu PATH") { |path| clufile = File.expand_path(path) }
  opts.on("-x", "--xpk PATH") { |path| xpkfile = File.expand_path(path) }
end
parser.parse!(ARGV)

if not clufile
  $stderr.puts("expecting a clu file!")
  exit 1
end

if not xpkfile
  $stderr.puts("expecting a xpk file!")
  exit 1
end

clu = CLU.from_file(clufile)
$stderr.puts(clu.inspect)

xpkfile = File.open(xpkfile, "r")

# these 2 seem to be the same header for all?
$stderr.puts xpkfile.read(16).unpack("C*").inspect
$stderr.puts xpkfile.read(8).unpack("C*").inspect
$stderr.puts xpkfile.read(8).unpack("C*").inspect
$stderr.puts("** ftell: #{xpkfile.tell}")
$stderr.puts("** togo: #{xpkfile.size - xpkfile.tell}")
pixeldata = xpkfile.read.unpack("C*")
xpkfile.close

pixeldata.each do |index|
  colors = clu.array[index]
  $stdout.print(colors.pack("C*"))
end

__END__
total = 22519; (1...total).each do |x| if total % x == 0 then p [x, total/x] end end
