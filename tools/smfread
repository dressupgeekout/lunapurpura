#!/usr/bin/env ruby
# smf reader

SMF_MAGIC = [173, 126, 121, 0] # ".~y\0"

require 'optparse'

path = ARGV.shift

if not ARGV
  $stderr.puts("expected an infile")
  exit 1
end

path = File.expand_path(path)

if not File.file?(path)
  $stderr.puts("#{path.inspect} is not a regular file")
  exit 1
end

char = "C*"
word = "S*"
word_be = "S>*"
dword = "L*"
dword_be = "L>*"

########## ########## ##########

smffile = File.open(path, "r")

puts "==== #{path} ===="

magic = smffile.read(4).unpack(char)
if magic != SMF_MAGIC
  $stderr.puts("invalid magic: #{magic.inspect} is not the expected #{SMF_MAGIC.inspect}")
  smffile.close
  exit 1
end

RECORD_MARKER = [0x00, 0x00, 0x7F, 0xFE]
record_type_a = [0x00, 0x02, 0x22, 0x84] # ?
record_type_b = [0x00, 0x0F, 0x3A, 0xF2] # ?
record_type_c = [0x30, 0x03, 0x00, 0x8F] # "a3DO"
record_type_d = [0x01, 0x01, 0x00, 0x00]
record_type_clip = [0x30, 0x00, 0x00, 0x8F]

smffile.read(4).unpack(char) == RECORD_MARKER || raise
type = smffile.read(2).unpack(word_be)[0]

if type == 2
  n_records = smffile.read(1).unpack(char)[0] / 2
  puts "#{n_records} records"
  offset = smffile.read(1).unpack(char)[0] / 2
  smffile.read(offset)
  smffile.seek(-10, :CUR)
  width = smffile.read(2).unpack(word_be)[0]
  height = smffile.read(2).unpack(word_be)[0]
  bit_depth = smffile.read(2).unpack(word_be)[0]
  audiolen_p = smffile.read(2).unpack(word_be)[0]
  channels = smffile.read(2).unpack(word_be)[0]
  sampling_rate = smffile.read(2).unpack(word_be)[0]
  puts "image dimensions = #{width}x#{height}"
  puts "audio: #{channels} channels, #{bit_depth}-bit @ #{sampling_rate} Hz"
end

# XXX
clip_lengths = [28, 44, 40, 44, 44, 40, 60, 36, 40, 56, 40, 52, 44]
clip = 0

## 1, 1, 1, 1, 1, 3, 1, 4, 2, 3, 3, 1, 4, 4ish

# minus 1 because we have already just read the 1st record
(n_records-1).times do |n|
  smffile.read(4).unpack(char) == RECORD_MARKER || raise

  type = smffile.read(4).unpack(char)

  case type
  when record_type_b
    puts "== RECORD TYPE B =="
    p smffile.read(16).unpack(word_be)
  when record_type_c
    codec = smffile.read(4)
    puts "== RECORD TYPE C =="
    puts "codec = #{codec.inspect}"
  when record_type_d
    puts "== RECORD TYPE D =="
    p smffile.read(36).unpack(word_be)
  when record_type_clip
    puts "== CLIP RECORD =="
    p smffile.read(clip_lengths[clip]).unpack(word_be)
    clip += 1
  end
end

=begin
CHUNKSIZE = (32*1024)-1

smffile.seek(892, :SET)
outf = File.open("/tmp/now.raw", "wb")

outf.write(smffile.read(CHUNKSIZE*1)) # audio
smffile.read(125647) # image
outf.write(smffile.read(CHUNKSIZE*1)) # audio

outf.close
=end
