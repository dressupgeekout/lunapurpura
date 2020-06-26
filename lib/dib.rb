#
# Purple Moon's DIB file format -- it's not quite a DIB because it's just a
# header that defines a bitmap of dimensions 1x1 (?!)
#
# https://msdn.microsoft.com/en-us/library/ms969901.aspx
#

require_relative 'clu'

class DIBFile < File
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

class DIB
  attr_reader :bfType
  attr_reader :bfSize
  attr_reader :bfReserved1
  attr_reader :bfReserved2
  attr_reader :bfOffBits
  attr_reader :biSize
  attr_reader :biWidth
  attr_reader :biHeight
  attr_reader :biPlanes
  attr_reader :biBitCount
  attr_reader :biCompression
  attr_reader :biSizeImage
  attr_reader :biXPelsPerMeter
  attr_reader :biYPelsPerMeter
  attr_reader :biClrUsed
  attr_reader :biClrImportant
  attr_reader :clu
  attr_reader :path

  def initialize(path)
    if not File.file?(path)
      raise ArgumentError, "#{path.inspect} is not a regular file"
    end

    @path = path

    DIBFile.open(@path, "r") do |f|
      # struct BITMAPFILEHEADER
      @bfType = f.read_word(nil) # "BM" magic
      @bfSize = f.read_dword.first # entire size of file
      @bfReserved1 = f.read_word.first # zeroes normally
      @bfReserved2 = f.read_word.first # zeroes normally
      @bfOffBits = f.read_dword.first # offset where the bits supposedly are

      # struct BITMAPINFOHEADER
      @biSize = f.read_dword.first # size of this struct (40, so it's a "Windows DIB")
      @biWidth = f.read_long.first
      @biHeight = f.read_long.first
      @biPlanes = f.read_word.first # should always be 1
      @biBitCount = f.read_word.first # bitsperpixel
      @biCompression = f.read_dword.first # compression enum
      @biSizeImage = f.read_dword.first # bytes in the bitmap
      @biXPelsPerMeter = f.read_long.first # application-defined value
      @biYPelsPerMeter = f.read_long.first # application-defined value
      @biClrUsed = f.read_dword.first # n colors in the CLUT (it is based on biBitCount)
      @biClrImportant = f.read_dword.first

      # The CLUT, all RGBQUADs, which are BGR and *not* RGB
      # XXX should be a Luna Purpura CLU struct?
      @clu = []
      256.times do |n|
        blue, green, red = f.read(3).unpack("C*") # Yes it's backwards
        @clu.push([red, green, blue])
        f.read(1) # reserved
      end
    end
  end

  def fancy
    s = []
    s << "bfType = " + @bfType.inspect
    s << "bfSize = " + @bfSize.to_s
    s << "bfReserved1 = " + @bfReserved1.inspect
    s << "bfReserved2 = " + @bfReserved2.inspect
    s << "bfOffBits = " + @bfOffBits.to_s
    s << "biSize = " + @biSize.to_s
    s << "biWidth = " + @biWidth.to_s
    s << "biHeight = " + @biHeight.to_s
    s << "biPlanes = " + @biPlanes.to_s
    s << "biBitCount = " + @biBitCount.to_s
    s << "biCompression = " + @biCompression.to_s
    s << "biSizeImage = " + @biSizeImage.to_s
    s << "biXPelsPerMeter = " + @biXPelsPerMeter.to_s
    s << "biYPelsPerMeter = " + @biYPelsPerMeter.to_s
    s << "biClrUsed = " + @biClrUsed.to_s
    s << "biClrImportant = " + @biClrImportant.to_s
    return s.join("\n")
  end

  # XXX need a way to make a clu on the fly, not from a file
  def fancy_clu
    clu_obj = CLU.new
    @clu.each { |pixel| clu_obj.array.push(pixel) }
    return clu_obj
  end
end
