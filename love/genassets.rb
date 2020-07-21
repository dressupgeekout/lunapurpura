#!/usr/bin/env ruby

require 'optparse'

prx_tool = nil
resource_dir = nil

parser = OptionParser.new do |opts|
  opts.on("--prx PATH", "path to prx(1)") { |path| prx_tool = File.expand_path(path) }
end

parser.parse!(ARGV)

resource_dir = ARGV.shift

if not resource_dir
  $stderr.puts("expected a path to a RESOURCE dir")
  exit 1
end

resource_dir = File.expand_path(resource_dir)

if not File.directory?(resource_dir)
  $stderr.puts("given path #{resource_dir.inspect} is not a directory")
  exit 1
end

$stdout.puts %Q(/* AUTOMATICALLY GENERATED ON #{Time.now.to_s} */)
$stdout.puts %Q(typedef uint32_t ASSET;)
$stdout.puts %Q()

Dir.glob("#{resource_dir}/**/*.PRX").each do |prx|
  cmd = "#{prx_tool} -t #{prx}"
  $stderr.puts("+ " + cmd)
  basename = File.basename(prx).sub(/\.PRX$/, "")
  IO.popen(cmd, :err=>[:child, :out]) do |stream|
    $stdout.puts %Q(enum #{basename} {)
    until stream.eof?
      line = stream.gets
      next if not line
      begin # XXX Nicole wall byte sequence problem
        next if line =~ /^\>\>/
        next if line =~ /^prx:/
      rescue
        next
      end
      elements = line.split(/\s+/)
      if elements.length != 5
        $stderr.puts("!! ENTRY WITH SPACE IN NAME! SKIPPING")
        next
      end
      id = elements[0]
      filename = elements[1]
      extension = elements[2]
      rid = elements[3]
      size = elements[4]
      $stdout.puts("\tASSET _#{filename.gsub(/[^A-Za-z0-9_\-]/, "")} = #{rid},")
    end
    $stdout.puts %Q(};)
    $stdout.puts %Q()
  end
end
