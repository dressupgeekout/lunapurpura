class CLU
  attr_reader :array

  def initialize(path)
    if not File.file?(path)
      raise ArgumentError, "#{path.inspect} is not a regular file"
    end
    
    clufile = File.open(path, "r")
    
    # header, I guess:
    # XXX should verify magic number
    clufile.read(8)
    
    @array = []
    
    256.times do |i|
      clufile.read(2).unpack("S*") # marker
      r = clufile.read(1).unpack("C*").first
      clufile.read(1) # Always a repeated R
      g = clufile.read(1).unpack("C*").first
      clufile.read(1) # Always a repeated G
      b = clufile.read(1).unpack("C*").first
      clufile.read(1) # Always a repeated B
      @array[i] = [r, g, b]
    end
    
    clufile.close
  end

  def eql?(other)
    @array.each_with_index do |pixel, i|
      return false if pixel != other.array[i]
    end
    return true
  end

  def fancy_text
    s = ""
    (0...16).each do |row|
      x = (0...16).map { |col| "[" + @array[row*16+col].join(",") + "]" }.join(", ")
      s << x << "\n"
    end
    return s
  end

  # Returns 3 values: R, G, B
  def color_at_index(i)
    return *@array[i]
  end
end
