f = File.open("./TRICKY/RTD_title.SMF", "r")

f.seek(816, :SET)
$stdout.puts(f.read(0x00008000)) # 1

f.seek(112224, :SET)
$stdout.puts(f.read(0x00008000)) # 2

f.seek(246308, :SET)
$stdout.puts(f.read(0x00008000)) # 3

f.seek(349605, :SET)
$stdout.puts(f.read(0x00008000)) # 4

f.seek(533166, :SET)
$stdout.puts(f.read(0x00008000)) # 5

f.seek(648753, :SET)
$stdout.puts(f.read(0x00008000)) # 6

f.seek(793086, :SET)
$stdout.puts(f.read(0x00008000)) # 7

f.seek(882320, :SET)
$stdout.puts(f.read(0x00008000)) # 8

f.seek(1035220, :SET)
$stdout.puts(f.read(0x00008000)) # 9

f.seek(1139250, :SET)
$stdout.puts(f.read(0x00008000)) # 10

f.seek(1250835, :SET)
$stdout.puts(f.read(0x0000420C))

f.close


len = 0x00008000

b = [
  816,
  112224,
  246308,
  349605,
  533166,
  648753,
  793086,
  882320,
  1035220,
  1139250,
]

b.each_cons(2) { |a, b| $stderr.puts b - (a + len) }



#f.seek(1250835, :SET)
#$stdout.puts(f.read(0x0000420C))


__END__

1317194 total

1316378 total (if we EXCLUDE header)
