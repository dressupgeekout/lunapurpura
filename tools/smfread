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

magic = smffile.read(4).unpack(char)
if magic != SMF_MAGIC
  $stderr.puts("invalid magic: #{magic.inspect} is not the expected #{SMF_MAGIC.inspect}")
  smffile.close
  exit 1
end

p smffile.read(8).unpack(char)

smffile.read(56) # zeroes??

w = smffile.read(2).unpack(word_be)[0]
h = smffile.read(2).unpack(word_be)[0]
puts "width=#{w} height=#{h}"

p smffile.read(4).unpack(word_be)

n_channels_maybe = smffile.read(2).unpack(word_be)[0]
sample_rate = smffile.read(2).unpack(word_be)[0]
puts "channels?=#{n_channels_maybe}"
puts "sample_rate=#{sample_rate} Hz"

puts "="*50 ########## ########## ##########

=begin
x = smffile.read(1284).unpack(dword_be)
x.each do |e|
  print e.to_s + " "
  puts if e == 32766 #2147364864 #
end

puts
=end

=begin
600.times do
  n = smffile.read(2).unpack(char)
  if n == [0x7F, 0xFE] or n == [0x0, 0x8F] or n == [0x80, 0x0]
    puts "-- (ftell=#{smffile.tell-2})"
    puts if n == [0x80, 0x0]
  end
  print "#{n}, "
end
=end

#KEYWORDS = {
#  "A" => [17, 0, 0, 143],
#  "B" => [17, 1, 0, 143],
#  "C" => [1, 1, 0, 0],
#  "D" => [0, 0, 128, 0],
#  "E" => [48, 0, 0, 143],
#  "F" => [0, 0, 127, 254],
#}

KEYWORDS = {
  "A" => [17, 0],
  "B" => [17, 1],
  "C" => [0, 143],
  "D" => [128, 0],
  "E" => [48, 0],
  "F" => [127, 254],
  "G" => [48, 3],
  "X" => [0, 1],
  "Y" => [1, 1],
  "Z" => [0, 0],
}

# YZZDAC
# YZZDBC
# ZFEC => X + [a,b] + FEC

=begin
until smffile.tell > 1300
  n = smffile.read(2).unpack(char)
  if KEYWORDS.values.include?(n)
    puts "-- (#{KEYWORDS.detect { |k, v| v == n }[0]}) ftell=#{smffile.tell}"
  else
    print "#{n.inspect}, "
  end
end
=end

=begin
until smffile.tell > 1300
  n = smffile.read(2).unpack(char)
  if n == KEYWORDS["X"]
    assoc = smffile.read(2)
    assoc_as_char = assoc.unpack(char)
    assoc_as_word = assoc.unpack(word_be)[0]
    print "-- (X)" + assoc_as_char.inspect + "=" + assoc_as_word.to_s
    if smffile.read(8).unpack(char) == (KEYWORDS["Z"] + KEYWORDS["F"] + KEYWORDS["E"] + KEYWORDS["C"])
      puts " :: (ZFEC)#{KEYWORDS["Z"].inspect}#{KEYWORDS["F"].inspect}#{KEYWORDS["E"].inspect}#{KEYWORDS["C"].inspect}"
    else
      smffile.seek(-8, :CUR)
    end
  else
    puts smffile.read(2).unpack(char).inspect
  end
end
=end

smffile.seek(124, :SET) # end of metadata??

collection = []
this = {}

#until smffile.tell > 1364
#  n = smffile.read(2).unpack(char)
#  if n == [0, 0]
#    this.push(n)
#    m = smffile.read(2).unpack(char)
#    if m == [0, 0]
#      collection.push(this)
#      this = [] # reset
#    else
#      this.push(m)
#    end
#  else
#    if n == [0, 1]
#      this.push(">>X<<")
#    else
#      this.push(n)
#    end
#  end
#end

=begin
until smffile.tell > 1364
  n = smffile.read(2).unpack(char)

  if n == [0, 0]
    if smffile.read(2).unpack(char) == [0, 0]
      collection.push(this)
      this = {} # RESET
    else
      this[:LEFTOVER] ||= []
      this[:LEFTOVER].push(n)
    end
  elsif n == [17, 0]
    a = smffile.read(2).unpack(char)
    this[:a] ||= []
    this[:a].push(a)
  elsif n == [17, 1]
    b = smffile.read(2).unpack(char)
    this[:b] ||= []
    this[:b].push(b)
  elsif n == [0, 1]
    x = smffile.read(2).unpack(word_be)[0]
    this[:x] ||= []
    this[:x].push(x)
  else
    this[:LEFTOVER] ||=  []
    this[:LEFTOVER].push(n) 
  end
end
=end

=begin =========
until smffile.tell > 822# 822 # 1364 1164
  n = smffile.read(4).unpack(char)
  
  if n == [0, 0, 128, 0]
    collection.push(this)
    this = {} # RESET
    next
  end

  a, b, c, d = n

  if [a, b] == [48, 0]
    this[:A] ||= []
    this[:A].push([c, d])
    next
  end

  # ALWAYS [17, 0, 0, 43]
  if [a, b] == [17, 0]
    this[:B] ||= []
    this[:B].push([c, d])
    next
  end

  # ALWAYS [17, 1, 0, 143] OR [17, 1, 0, 0]
  if [a, b] == [17, 1]
    this[:C] ||= []
    this[:C].push([c, d])
    next
  end

  if [a, b] == [0, 0]
    this[:D] ||= []
    this[:D].push([c, d].pack(char).unpack(word_be)[0])
    next
  end

  if [a, b] == [33, 0]
    this[:E] ||= []
    this[:E].push([c, d])
    next
  end

  if [a, b] == [0, 1]
    this[:X] ||= []
    this[:X].push([c, d].pack(char).unpack(word_be)[0])
    next
  end

  if [a, b] == [1, 1]
    this[:Y] ||= []
    this[:Y].push([c, d])
    next
  end

  this[:LEFTOVER] ||= []
  this[:LEFTOVER].push(n)
end
=end =========

markercounter = 0
things = []
this_thing = {}

#808 1364 #1164
MAGIC_POSITION = 1364 # not sure how to compute this just yet

until smffile.tell >= MAGIC_POSITION
  x = smffile.read(4).unpack(char)
  a, b, c, d = x

  if x == [0, 0, 128, 0]
    puts ">> MARKER ##{markercounter}"
    markercounter = markercounter + 1
    things << this_thing
    this_thing = {}
    next
  end

  if [a, b] == [17, 0]
    raise if d != 143
    puts "FLAG1->OFF #{c},#{d}"
    this_thing[:flag_off] ||= 0
    this_thing[:flag_off] = this_thing[:flag_off] + 1
    next
  end

  if [a, b] == [17, 1]
    raise if not [0, 143].include?(d)
    puts "FLAG1->ON #{c},#{d}"
    this_thing[:flag_on] ||= 0
    this_thing[:flag_on] = this_thing[:flag_on] + 1
    next
  end

  if [a, b] == [0, 0]
    n = [c, d].pack(char).unpack(word_be)[0]
    puts "<INTEREST?> " + n.to_s
    this_thing[:interesting] ||= []
    this_thing[:interesting] << n
    next
  end

  if [a, b] == [1, 1]
    if [c, d] == [0, 0]
      # XXX this isn't the only "END" combo i've seen
      puts ">> END (ftell=#{smffile.tell})"; puts
      next
    end
  end
  
  puts "(extra) " + x.inspect
  this_thing[:extra] ||= []
  this_thing[:extra] << x
end

puts "="*50 ########## ########## ##########

things.each { |thing| p thing; puts }
p things.length

puts "="*50 ########## ########## ##########

smffile.seek(MAGIC_POSITION+2, :SET)

File.open("./out.raw", "wb") { |outf|
  things.each_with_index do |thing, i|
    next if thing.empty? # my fault, from above
    length = thing[:interesting][1] || thing[:interesting][0]
    n = thing[:extra].select { |e| e ==  [48, 0, 0, 143] }.length # ?? random guess
    puts ">> #{length.inspect} x #{n}"
    n.times do
      buf = smffile.read(length)
      outf.write(buf)
    end
  end
}

smffile.close
