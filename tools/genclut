#!/usr/bin/env ruby

require 'optparse'
require 'stringio'

cluread_prog = `which cluread`.chomp
clufile_path = nil
square_size = 32

parser = OptionParser.new do |opts|
  opts.on("--cluread PROGRAM") { |prog| cluread_prog = File.expand_path(prog) }
end
parser.parse!(ARGV)

clufile_path = ARGV.shift

if not clufile_path
  $stderr.puts("expected a path to a CLU file")
  exit 1
end

clufile_path = File.expand_path(clufile_path)

if not File.file?(clufile_path)
  $stderr.puts("no such file #{clufile_path.inspect}")
  exit 1
end

fd = StringIO.new(`#{cluread_prog} #{clufile_path} 2>&1`)

2.times { fd.gets } # skip this

colors = Array.new(256)

256.times do |i|
  line = fd.gets.chomp.split("\t")
  colors[i] = eval(line.last) # XXX lol
end

16.times.each do |colorgroup_begin|
  colorgroup = colors[16*colorgroup_begin, 16]

  square_size.times do |square_y|
    colorgroup.each do |color|
      square_size.times do |square_x|
        $stdout.print(color.pack("C*"))
      end
    end
  end
end

$stderr.puts("resulting image resolution: #{square_size*16}x#{square_size*16}")
