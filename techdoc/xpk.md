# XPK format

An XPK file contains 1 or more raster graphics. When there are multiple
"sub-images," they typically form one logical unit, either

- several "chunks," "sections" or "regions" of a larger image, or
- several frames of a single animation.


## Things we know about XPK files

- They define 1 or more rasters of pixels. 
- They specify the width and height of the image.
- Pixels are indexed with a corresponding color lookup table (CLU) -- XPK
  files do not "spell out" RGB values.
- A form of run-length encoding (RLE) is present, but it likely is not the
  only supported form of compression. 


## Things we DON'T know about XPK files

- How to properly decode them?
- How to definitively identify whether a given byte represents a
  color index, or a count for RLE purposes, or something else?
- Whether they're identical in nature throughout all _Rockett_ titles
