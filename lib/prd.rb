# occurence_id -- this entry's absolute position within the order of all entries in ths file
# id -- the "GUID"
# internal_id -- this entry's given position among all entries in this file
# size -- in bytes
# ext -- always 3 characters
# offset -- where it's located in the PRS file
# name -- max 18 chars
class Entry < Struct.new(:occurence_id, :id, :internal_id, :size, :ext, :offset, :name)
end

class PRD
  MAGIC = [1, 0, 0, 0]

  def initialize(path)
    if not File.file?(path)
      raise ArgumentError, "no such file #{path.inspect}"
    end
  end
end
