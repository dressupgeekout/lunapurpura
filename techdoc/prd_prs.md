# PRD/PRS format

_Rockett's New School_ is the only game in the _Rockett_ series which
stores all its content in combination PRD/PRS archives, rather than the
combined PRX archives of the later games.

In essence, the PRD file is a header which specifies a table of contents and
lists a bunch of offsets, and the PRS file contains the actual asset data.

The strategy involved in decoding PRD/PRS is almost identical to that of a
PRX archive.  Thus, the PRD part and the PRS part can effectively be
concatenated to form a valid PRX file -- some modifications are required, of
course (offsets will be all different, for starters) but the gist is
definitely true.


## Game compatibility list

- _Rockett's New School_ ONLY
