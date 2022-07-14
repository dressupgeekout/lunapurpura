#!/usr/bin/env ruby

require 'tempfile'

$LOAD_PATH.unshift File.join(__dir__, "..", "lib")
require 'clu'

ENTRY_SIZE = 32 # pixels square

path = ARGV.shift
clu = CLU.new(path)

rgbfile = Tempfile.new(File.basename($0))

(0...16).each do |row|
  ENTRY_SIZE.times do |n|
    (0...16).each do |column|
      index = row*16+column
      this_cell = []
      ENTRY_SIZE.times { this_cell.push(*clu.array[index]) }
      rgbfile.write(this_cell.pack("C*"))
    end
  end
end

rgbfile.close
png_data = `gm convert -size #{ENTRY_SIZE*16}x#{ENTRY_SIZE*16} -depth 8 rgb:#{rgbfile.path} png:-`
rgbfile.unlink
$stdout.write(png_data)
