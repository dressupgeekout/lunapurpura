#!/usr/bin/env ruby
#
# fakesqs2
#
# This file is part of Luna Purpura.
#
# This script takes an AIFF-C file (called .AIF or .Aif in Purple Moon games)
# with a COMM chunk of "SQS2", and saves a new file by replacing the COMM chunk
# a compression scheme of your choosing. By default, "SDX2" is used, for its
# close similarity to SQS2.
#
# Until the SQS2 compression format can finally be decoded, you'll need this
# script in order to be able to hear all sounds in all Rockett games.
#

require 'fileutils'
require 'optparse'
require 'tempfile'

PROGNAME = File.basename($0)

replace = false
outfile = nil
infile = nil
new_comm = "SDX2"

parser = OptionParser.new do |opts|
  opts.banner = "usage: #{PROGNAME} [options ...] file"
  opts.on("-c", "--comm FOURCC", "replace SQS2 w/ given FOURCC (default: #{new_comm})") { |c| new_comm = c }
  opts.on("-i", "--in-place", "overwrite input file") { replace = true }
  opts.on("-o", "--outfile PATH", "path to write new file") { |o| outfile = File.expand_path(o) }
end
parser.parse!(ARGV)

infile = ARGV.shift

if not infile
  $stderr.puts(parser.to_s)
  exit 1
end

infile = File.expand_path(infile)

if replace && outfile
  $stderr.puts("can't overwrite in-place and also specify a new outfile")
  exit 1
end

if !replace && !outfile
  $stderr.puts(parser.to_s)
  exit 1
end

if new_comm.length != 4
  $stderr.puts("#{PROGNAME}: New COMM specifier #{new_comm.inspect} is not exactly 4 characters long")
  exit 1
end

# It's true that one can do I/O on a Tempfile object but I just want its
# path.
tempfile = Tempfile.new
tempfile.close

FileUtils.cp(infile, tempfile.path)

# This is the thing we're trying to find:
compressionTypePos = -1

File.open(tempfile.path, "r") do |f|
  until f.eof?
    ckID = f.read(4).unpack("Z*")[0]
    puts "Found chunk #{ckID.inspect}"

    case ckID
    when "FORM"
      ckDataSize = f.read(4).unpack("L>*")[0]
      ckStartPos = f.tell
      formType = f.read(4).unpack("Z*")[0]
      if formType != "AIFC"
        # No modifications needed
        break
      end
    when "COMM"
      ckDataSize = f.read(4).unpack("L>*")[0]
      ckStartPos = f.tell
      f.read(2) # numChannels
      f.read(4) # numSampleFrames
      f.read(2) # sampleSize
      f.read(10) # sampleRate
      compressionType = f.read(4).unpack("Z*")[0]
      compressionTypePos = f.tell - 4
      if compressionType == "SQS2"
        # Jackpot
        puts "Found compressionType #{compressionType.inspect} @ #{compressionTypePos}"
        break
      else
        # No modifications needed
        break
      end
    else
      ckDataSize = f.read(4).unpack("L>*")[0]
      ckStartPos = f.tell
    end

    f.seek(ckStartPos + ckDataSize) if ckID != "FORM"
  end
end

if compressionTypePos > 0
  in_f = File.open(infile, "r")
  out_f = File.open(tempfile.path, "w")

  # Copy the input file directly, up to where the compression marker is
  before = in_f.read(compressionTypePos)
  out_f.write(before)

  # Now we slip in our own compression marker:
  out_f.write(new_comm)

  # And now the remainder of the file is the same.
  in_f.seek(4, :CUR)

  after = in_f.read
  out_f.write(after)

  in_f.close
  out_f.close
end

if replace
  if compressionTypePos > 0
    FileUtils.mv(tempfile.path, infile)
    puts "Replaced #{infile} with #{new_comm.inspect} compressionType"
  else
    puts "No modifications needed"
  end
else
  FileUtils.mv(tempfile.path, outfile)
  if compressionTypePos > 0
    puts "Wrote #{outfile} with #{new_comm.inspect} compressionType"
  else
    puts "No modifications needed"
  end
end
