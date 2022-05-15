#!/usr/bin/env ruby
#
# This file is part Luna Purpura.
#
# Use this tool to list and extract files from PRD+PRS files as found in
# Rockett's New School. You will need to have both the header in the PRD
# file *and* the data in the PRS file handy.
#
# It's up to you to make sure the PRD you're providing truly does correspond to
# the PRS file; there's no way for this tool to know for certain whether that's
# the case. (Although it tries to guess.)
#

require 'fileutils'
require 'optparse'

which_asset = nil
$inspect = false
do_names = false
prdpath = nil
prspath = nil
extracting = false

parser = OptionParser.new do |opts|
  opts.on("-h", "--help") { puts opts.to_s; exit 0 }
  opts.on("--asset N") { |n| which_asset = n.to_i }
  opts.on("--inspect") { $inspect = true }
  opts.on("--names") { do_names = true }
  opts.on("--prd", "--header PATH") { |path| prdpath = File.expand_path(path) }
  opts.on("--prs", "--data PATH") { |path| prspath = File.expand_path(path) }
  opts.on("-x", "--extract") { extracting = true }
end
parser.parse!(ARGV)

{"PRD file" => prdpath, "PRS file" => prspath,}.each do |name, var|
  if not var
    $stderr.puts("expected #{name}")
    exit 1
  end

  if not File.file?(var)
    $stderr.printf("Given %s (%s) is not a regular file\n", name, var)
    exit 1
  end
end

prdfile = File.open(prdpath, "r")

char1 = prdfile.read(1).unpack("C*").first
if char1 != 1
  $stderr.printf("(header1) first character is not 1, got %d\n", char1)
  prdfile.close
  exit 1
end

prdfile.read(137) # 128 + 10 - 1, i guess -- all zeroes

# weird that '_unknown' is *ONLY SOMETIMES* not equal to $n
_unknown, $n = prdfile.read(4).unpack("S*") # uint16_t
$stderr.puts([_unknown, $n].inspect)
prdfile.read(2) # padding, i guess 

########## ########## ##########

# occurence_id -- this entry's absolute position within the order of all entries in ths file
# id -- the "GUID"
# internal_id -- this entry's given position among all entries in this file
# size -- in bytes
# ext -- always 3 characters
# offset -- where it's located in the PRS file
# name -- max 18 chars
class Entry < Struct.new(:occurence_id, :id, :internal_id, :size, :ext, :offset, :name)
end

$entries = []

########## ########## ##########

total_size = 0
$size_map = {}

($n+1).times do |x|
  internal_id = prdfile.read(4).unpack("S*").first
  prdfile.read(4) # zeroes
  offset = prdfile.read(4).unpack("L*").first
  format = prdfile.read(4).strip # 3 characters including NUL, so we strip it
  some_id = prdfile.read(4).unpack("L*").first
  size_of_this_asset = prdfile.read(4).unpack("L*").first
  $size_map[x] = size_of_this_asset
  total_size = total_size + size_of_this_asset
  $entries[x] = Entry.new(x, some_id, internal_id, size_of_this_asset, format, offset, nil)
end

$stderr.puts "="*50
$entries.each do |entry|
  $stderr.puts(entry.inspect)
end
$stderr.puts "="*50

$stderr.puts
$stderr.puts "ftell: #{prdfile.tell}"
$stderr.puts "TOTAL SIZE OF ALL ASSETS?: #{total_size} = #{total_size/1024} KB"
$stderr.puts

def names(prsfile)
  (1..$n).each do |asset|
    this_entry = $entries[asset]

    # the 'offset' field in the Entry actually points 4 bytes AHEAD of where
    # the data actual begins (sometimes???). so go back 4. but also we go back
    # 20 more to obtain the metadata about this particular entry, including its
    # name.
    prsfile.seek(this_entry.offset - 24, IO::SEEK_SET) 

    # *** vv info we're skipping because we just want names vv ***
    ###$stdout.puts(prsfile.read(16).gsub("\000", "")) # 16-char name, including nul
    ###prsfile.read(15) # idk what this is 
    ###x = prsfile.read(1).unpack("C*") # idk
    ###prsfile.read(16) # zeroes i think
    ###prsfile.read(4) # format in ASCII again for some reason, incl. nul

    prsfile.read(1) # dash
    name_len_sortof = prsfile.read(1).unpack("c").first # seems to work sometimes but not others...
    name = prsfile.read(18).strip
    this_entry.name = name
    $stderr.puts("#{asset}: #{this_entry.ext}\t#{name}")
  end
end

def seek_and_read(prsfile, which_asset)
  entry = $entries[which_asset]
  prsfile.seek(entry.offset, IO::SEEK_SET)
  return prsfile.read(entry.size)
end

prsfile = File.open(prspath, "r")

if do_names
  names(prsfile)
else
  if which_asset
    data = seek_and_read(prsfile, which_asset)

    if $inspect
      $stdout.puts(data.inspect)
    else
      $stdout.print(data)
      $stdout.flush
    end
  elsif extracting
    extract_dir = File.join(Dir.pwd, File.basename(prsfile, ".PRS"))
    if File.exist?(extract_dir)
      $stderr.puts("FATAL: there already exists something at #{extract_dir}")
      exit 1
    end
    names(prsfile) # compute the names -- XXX should be a separate thing
    FileUtils.mkdir_p(extract_dir)
    (1..$n).each do |asset|
      entry = $entries[asset]
      data = seek_and_read(prsfile, asset)
      this_path = File.join(extract_dir, "#{entry.name}.#{entry.ext}")
      File.open(this_path, "w") { |f| f.write(data) }
      $stderr.puts(this_path)
    end
  end
end

prdfile.close
prsfile.close
