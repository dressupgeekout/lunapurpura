# SMF format

SMF is "the" animation format in the _Rockett_ series. They appear to
contain a combination of graphical data and audio data. They presumably also
contain timing data so that we know how long to display the current frame
and when to move on to the next frame.


## Things we know about SMF

- They all have the same magic number.
- 1 SMF stream alternates between audio data and non-audio data (metadata
  and/or graphics.
- The format of the audio aligns with the rest of the audio used in the rest
  of the game. Specifically, it seems to be "SQS2"-ified in _Tricky
  Decision_ but ordinary 8-bit mono PCM @ 22050 Hz in the other titles.


## Things we DON'T know about SMF

- How to decode them properly.
- Where the timing information for images is stored, if it even _is_ stored
  here
- How to precisely identify where the audio regions are located, given the
  metadata at the beginning (there don't seem to be any offsets stored
  within)
- How to precisely identify where the non-audio regions are located, given
  the metadata at the beginning (there don't seem to be any offsets stored
  within)
- What the non-audio regions even contain -- are they XPK graphics, or
  something else?
