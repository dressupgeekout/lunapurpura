#
# Purple Moon's PRX file format
#

class PRXFile < File
  # WORD is 16 bits (2 bytes) unsigned
  def read_word(unpack_method="C*")
    red = self.read(2)
    if unpack_method
      return red.unpack(unpack_method)
    else
      return red
    end
  end

  # DWORD is 32 bits (4 bytes) unsigned
  def read_dword(unpack_method="L*")
    red = self.read(4)
    if unpack_method
      return red.unpack(unpack_method)
    else
      return red
    end
  end

  def read_long(unpack_method="l*")
    red = self.read(4)
    if unpack_method
      return red.unpack(unpack_method)
    else
      return red
    end
  end
end

class PRX
  attr_reader :path

  def initialize(path)
    if not File.file?(path)
      raise(ArgumentError, "#{path.inspect} is not a regular file")
    end

    @path = path

    PRXFile.open(@path, "r") do |f|
    end
  end

  def names
  end
end
