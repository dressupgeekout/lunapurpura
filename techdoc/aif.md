# Aif files and SQS2 compression

All audio data in the _Rockett_ series of games evaluated thus far are PCM
audio data. In the majority of cases, it's mono, 8-bit signed integer PCM at
22050 Hz. There really isn't anything magical there. We can already extract
a lot of game audio without a problem!

**But one game** (_Tricky_) features this gnarly, super rare compressed
format (DPCM or ADPCM) that no one understands! It's called "SQS2". So, the
rest of this article is dedicated to understanding SQS2.


## Things we know about SQS2-compressed audio

- They're AIF-C files with a COMM chunk of "SQS2". We already know how to
  decode AIF-C files in general, it's just this magical COMM value no one
  understands.
- It shows up only in _Tricky Decision_, no other Purple Moon title
  evaluated thus far.
- It shows up in characters' "IDL" PRX archives and also SMF animations.
- "SQS2" is (most likely) a reference to the tool which was commonly used to
  generate data of this type, SquashSound, and not any indication of the
  mathematics used to decode it (e.g. "SDX2" corresponds to
  Square-Root-Delta). (XXX provide a pointer to the 3DO documentation I
  found)
- The 3DO company is definitely involved. The string "3DO" appears all over
  the place and, as far as we can tell, the SQS2 format is their invention.
- They're all mono, just like the uncompressed _Rockett_ audio from other
  games.
- They're all played at 22050 Hz, just like the uncompressed _Rockett_ audio
  from other games.

## Thing we DON'T know about SQS2-compressed audio

- How to decode them?!!


## Game compatibility list

### Mono 8-bit signed integer PCM @ 22050 Hz

- _Rockett's New School_
- _Rockett's Secret Invitation_


### AIFF-C with "SQS2" COMM chunk:

- _Rockett's Tricky Decision_
