#!/usr/bin/env ruby

require 'optparse'

want_extract = false

parser = OptionParser.new do |opts|
  opts.on("-x", "--extract") { want_extract = true }
end
parser.parse!(ARGV)

mmfwfile = ARGV.shift

if not mmfwfile
  $stderr.puts(parser.to_s)
  exit 1
end

mmfwfile = File.expand_path(mmfwfile)

if not File.file?(mmfwfile)
  $stderr.puts("no such file: #{mmfwfile.inspect}")
  exit 1
end

f = File.open(mmfwfile, "r")

MAGIC = "MMFW"

magic = f.read(4).unpack("Z*")[0]

if magic != MAGIC
  $stderr.puts("bad magic: #{magic.inspect} != #{MAGIC.inspect}")
  exit 1
end

f.read(1) # space char (ascii 20)

description = f.read(7).unpack("Z*")[0]
puts "description = #{description.inspect}"

f.read(4) # zeroes
f.read(4) # [77, 77, 0, 2]
f.read(14) # not sure

n_elements = f.read(2).unpack("S>*")[0]
puts "n_elements = #{n_elements}"

offsets = []

n_elements.times do |i|
  offsets << f.read(4).unpack("L>*")[0]
end

total_file_size = f.read(4).unpack("L>*")[0]
puts "total_file_size = #{total_file_size}"
offsets << total_file_size

titles = []

n_elements.times do |i|
  f.read(2) # zeroes
  title = f.read(30).unpack("Z*")[0]
  titles << {
    :title => title,
  }
end

f.read(2)

n_elements.times do
  f.read(4) # XXX not sure what these are
end

f.read(2) # XXX  dunno
puts f.tell

n_elements.times do |i|
  titles[i][:type] = f.read(1).unpack("C*")[0]
end

titles.each_with_index do |member, i|
  typemap = {
    0x1D => "mono s16be @ 44100 Hz",
    0x15 => "mono s16be @ 22050 Hz",
  }
  printf "%-30s\t%s\t%d\n", member[:title], typemap[member[:type]], offsets[i]
end

if want_extract
  n_elements.times do |i|
    offset = offsets[i]
    length = offsets[i+1] - offset
    title = titles[i][:title]
    f.seek(offset, :SET)
    puts "x\t#{title}"
    File.open(title, "w") { |outf|
      outf.write(f.read(length))
    }
  end
end

f.close
