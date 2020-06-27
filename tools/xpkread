#!/usr/bin/env ruby
# XPK reader WIP

require_relative '../lib/clu'
require 'optparse'
require 'fileutils'

XPK_MAGIC = [165, 126, 112, 1]
$make_movie = false

which = 0
clu_path = nil

parser = OptionParser.new do |opts|
  opts.on("-n", "--which N") { |n| which = n.to_i }
  opts.on("-c", "--clu CLU") { |c| clu_path = File.expand_path(c) }
end
parser.parse!(ARGV)

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

########## ########## ##########

xpkfile = File.open(path, "r")

magic = xpkfile.read(4).unpack("C*")

if magic != XPK_MAGIC
  $stderr.puts("magic #{magic.inspect} does not match expected magic: #{XPK_MAGIC.inspect}")
  exit 1
end

# XXX Is this part of the magic?
co2 = xpkfile.read(2).unpack("C*")
$stderr.puts co2.inspect

$stderr.puts "="*50 ##### ##### ##### ##### ##### #####

# total number of entries
n = xpkfile.read(2).unpack("S>*").first
$stderr.puts "n = #{n}"

a = xpkfile.read(4).unpack("S>*")
$stderr.puts "a = #{a}"

next_section = xpkfile.read(4).unpack("L>*").first
$stderr.puts "next_section = #{next_section}"

segments = []

# I believe this is the width and height of each section -- but each one's
# specified twice for some reason
#
# XXX where did I come up with the "twice" thing?
n.times do
  x1, x2, w1, h1, x, y, w2, h2 = xpkfile.read(16).unpack("S>*")
  str = "#{x1},#{x2} - #{w1}x#{h1} - pos=#{x},#{y} - dimensions=#{w2}x#{h2}"
  segments.push([str, w2, h2])
end

$stderr.puts "segments:"

segments.each do |segment|
  $stderr.puts "\t- #{segment}"
end

$stderr.puts "="*50 ##### ##### ##### ##### ##### #####

OFFSETS_START_POS = xpkfile.tell

offsets = []

n.times do
  this = xpkfile.read(4).unpack("L>").first
  offsets.push(this)
end

offsets.each do |off|
  $stderr.puts "offset: #{off.to_s}"
end

$stderr.puts "="*50 ##### ##### ##### ##### ##### #####

offsets.each_with_index do |off, i|
  $stderr.puts "// offset_maybe # #{i} //"
  xpkfile.seek(OFFSETS_START_POS + off, :SET)
  $stderr.puts [xpkfile.tell, xpkfile.read(2).unpack("C*")].inspect
end

$stderr.puts "="*50 ##### ##### ##### ##### ##### #####

MAP = {}

offset_index = 0

xpkfile.seek(OFFSETS_START_POS + offsets[offset_index], :SET)

first_number = {}

until xpkfile.eof?
  this = [] # reset this
  char = -1

  until xpkfile.eof?
    char = xpkfile.read(1).unpack("C").first
    this << char
  end

  # OK, need to reset
  puts "(#{offset_index})\t#{this.map {|x| MAP[x] or x }.inspect}\t\t=> #{this.length}"
  2.times { puts }

  if offset_index < offsets.length
    if xpkfile.tell > (OFFSETS_START_POS+offsets[offset_index])
      offset_index = offset_index + 1
    end
  end

  first_number[this[0]] ||= 0
  first_number[this[0]] = first_number[this[0]] + 1
end

$stderr.puts "="*50 ##### ##### ##### ##### ##### #####

first_number.sort_by { |k, v| -v }.each do |k, v|
  puts "\tfirst=#{k} => #{v} times"
end

$stderr.puts "="*50 ##### ##### ##### ##### ##### #####

xpkfile.seek(OFFSETS_START_POS+offsets[0], :SET)
p xpkfile.tell

clu = CLU.new(clu_path)

entry = segments[which]
w = entry[1]
h = entry[2]

File.open("attempt.rgba", "w") do |f|
  $stderr.puts("trying to create an image #{w}x#{h}")

  cur_x = 0
  cur_y = 0

  # Start with a transparent grid, we'll fill it in as we go along.
  matrix = []

  h.times do
    matrix.push(Array.new(w, [0, 0, 0, 0]))
  end

  n_reps = 0
  which_color = 0
  direct_counter = 0
  meta_repeat = 0
  meta_repeat_loc = 0
  line_repeat = 0
  line_repeat_loc = 0

  if $make_movie
    frames_dir = "/tmp/frames"
    FileUtils.mkdir_p(frames_dir)
    puts "**** DIRECTORY = #{frames_dir} *****"
    frame = 0
  end

  until xpkfile.eof?
    p [cur_x, cur_y, "ftell=#{xpkfile.tell}"] if $DEBUG

    if $make_movie
      frame = frame + 1
      frame_f = File.join(frames_dir, sprintf("%08d.rgb", frame))

      File.open(frame_f, "w") { |frame_fp|
        h.times do |y|
          w.times do |x|
            pixel = matrix[y][x]
            frame_fp.write(pixel.pack("C*"))
          end
        end
      }

      system %Q(gm convert -size #{w}x#{h} -depth 8 rgba:#{frame_f} #{frame_f.sub('.rgba', '.png')})
      FileUtils.remove_entry(frame_f)
    end

    char = xpkfile.read(1).unpack("C")[0]

    # XXX HACK!!
    cur_y = (h-1) if cur_y >= h # XXX HACK!!

    if direct_counter > 0 # DIRECT MODE ENABLED
      which_color = char
      r, g, b = clu.color_at_index(which_color)
      matrix[cur_y][cur_x] = [r, g, b, 255]
      cur_x = cur_x + 1
      puts ">> DIRECT #{which_color}" if $DEBUG
      direct_counter = direct_counter - 1
      if direct_counter == 0
        puts ">> (DIRECT MODE EXPIRED)" if $DEBUG
      end
    else  # DIRECT MODE OFF OR EXPIRED
      if n_reps > 0 # RLE engaged
        which_color = char
        r, g, b = clu.color_at_index(which_color)
        while n_reps > 0
          matrix[cur_y][cur_x] = [r, g, b, 255]
          cur_x = cur_x + 1
          n_reps = n_reps - 1
        end
      else # NEED A NEW INSTRUCTION
        operation = char >> 4 # first 4 bits
        argument = char & 0x0F # last 4 bits

        case operation
        when 0x0
          if argument == 0x0
            n_reps = xpkfile.read(1).unpack("C")[0]
            puts ">> RLE #{n_reps}" if $DEBUG
          elsif argument == 0x1
            meta_repeat = meta_repeat - 1
            if meta_repeat > 0
              xpkfile.seek(meta_repeat_loc, :SET) # Go back to the repeat section
              puts ">> ENDREPEAT: BACK TO START: #{meta_repeat} remaining" if $DEBUG
            else
              meta_repeat_loc = 0
              puts ">> DONE REPEATING" if $DEBUG
            end
          else
            meta_repeat = argument
            meta_repeat_loc = xpkfile.tell
            puts ">> BEGIN REPEATING for #{meta_repeat} times" if $DEBUG
          end
        when 0x2
          cur_x = cur_x + argument
        when 0x3
          cur_x = cur_x + 16 + argument
        when 0x4
          case argument
          when 0x0
            # OK to ignore
          when 0x1
            # EOF!
            break
          else
            n_reps = argument
            puts ">> RLE #{n_reps}" if $DEBUG
          end
        when 0x5
          n_reps = 16+argument
          puts ">> RLE #{n_reps}" if $DEBUG
        when 0x6
          direct_counter = argument
          puts ">> DIRECT MODE ENGAGED for #{direct_counter} pixels" if $DEBUG
        when 0x7
          direct_counter = 16+argument
          puts ">> DIRECT MODE ENGAGED for #{direct_counter} pixels" if $DEBUG
        when 0x8
          if argument == 0x0
            # XXX I'm not sure if that makes sense
          elsif argument == 0x1
            line_repeat = line_repeat - 1
            if line_repeat > 0
              xpkfile.seek(line_repeat_loc, :SET) # Go back to the repeat section
              puts ">> ENDLINEREPEAT: BACK TO START: #{line_repeat} remaining" if $DEBUG
            else
              line_repeat_loc = 0
              puts "> DONE LINEREPREATING" if $DEBUG
            end
            cur_x = 0
            cur_y = cur_y + 1
          else
            cur_x = 0
            cur_y = cur_y + 1
            line_repeat = argument
            line_repeat_loc = xpkfile.tell
            puts ">> BEGIN LINE-REPEAT for #{line_repeat} times" if $DEBUG
          end
        when 0xA
          cur_x = argument
          cur_y = cur_y + 1
          puts ">> NEWLINE AND ADVANCE: --> X=#{cur_x}" if $DEBUG
        when 0xB
          cur_x = 16+argument
          cur_y = cur_y + 1
          puts ">> NEWLINE AND ADVANCE: --> X=#{cur_x}" if $DEBUG
        when 0xC
          n_reps = argument
          cur_x = 0
          cur_y = cur_y + 1
          puts ">> NEWLINE + RLE #{n_reps}" if $DEBUG
        when 0xD
          n_reps = 16+argument
          cur_x = 0
          cur_y = cur_y + 1
          puts ">> NEWLINE + RLE #{n_reps}" if $DEBUG
        when 0xE
          direct_counter = argument
          cur_x = 0
          cur_y = cur_y + 1
          puts ">> NEWLINE + DIRECT MODE ENGAGED for #{direct_counter} pixels" if $DEBUG
        when 0xF
          direct_counter = 16+argument
          cur_x = 0
          cur_y = cur_y + 1
          puts ">> NEWLINE + DIRECT MODE ENGAGED for #{direct_counter} pixels" if $DEBUG
        end
      end
    end
  end

  # Write RGB file
  h.times do |y|
    w.times do |x|
      pixel = matrix[y][x]
      f.write(pixel.pack("C*"))
    end
  end
end # File.open()

# Convert to PNG
cmd = %Q(gm convert -depth 8 -size #{w}x#{h} rgba:attempt.rgba #{File.basename(xpkfile.path)}.png)
$stderr.puts(cmd)
system cmd
