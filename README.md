# Luna Purpura

_**This software is totally a work in progress and very few things are
actually functional.  It's still in a research phase.**_

**Luna Purpura** is a portable reimplementation of [Purple Moon][]'s classic
computer games from the 1990s:

- [_Rockett's New School_](https://en.wikipedia.org/wiki/Rockett%27s_New_School)
- _Rockett's Tricky Decision_
- _Rockett's Secret Invitation_
- ...and more

It is a modern interpreter of the original game files. The goal is: if you
have the original CD-ROMs, then you can play these games on today's
computers, even on platforms which Purple Moon did not originally support.
You should also be able to use the engine to make your own games, too!

Support for _Secret Paths in the Forest_ and the other _Secret Paths_ games
is planned, but not currently being worked on. Get in touch if you'd like to
help out!


## Building Luna Purpura

In order to build games and support libraries from Luna Purpura, you must
have CMake.

```sh
$ mkdir build
$ cd build
$ cmake .. [cmake-options]
$ make
```

The following CMake configuration options are supported:

- BOOL `LUNAPURPURA_DEBUG` -- Enable debug output
- BOOL `LUNAPURPURA_BUILD_LUA_BINDINGS` -- Build the Lua 5.1 (or LuaJIT) bindings
- BOOL `LUNAPURPURA_BUILD_MRUBY_BINDINGS` -- Build the mruby bindings
- BOOL `LUNAPURPURA_BUILD_TESTS` -- Build the test applications
- BOOL `LUNAPURPURA_PNG_SUPPORT` -- Enable conversion to PNG

So, for example:

```
$ cmake .. -DLUNAPURPURA_DEBUG=ON -DLUNAPURPURA_PNG_SUPPORT=OFF
```

If you turn on the Lua bindings, you need to also define the following:

- PATH `LUA_INCLUDEDIR` -- Location of Lua 5.1/LuaJIT's header files
- PATH `LUA_LIBDIR` -- Location of Lua 5.1/LuaJIT's library

If you turn on the mruby bindings, you need to also define the following:

- PATH `MRUBY_INCLUDEDIR` -- Location of mruby's header files
- PATH `MRUBY_LIBDIR` -- Location of mruby's library

If you turn on PNG support (enabled by default), then you must have the
libraries and development headers for zlib and libpng installed.


### build.sh Usage

For the most basic use cases, we provide an easy script that does it all for
you:

```sh
$ ./build.sh [make(1) args ...]
```

You can force a fresh, clean rebuild with the 'clean' argument:

```sh
$ ./build.sh clean
```


## Using the tools

Currently, the most feature-complete software are the Ruby scripts in the
`tools/` directory. Most versions of Ruby >= 2.3 should work just fine on
them.  Note that these are merely prototypes which are designed to make
research on Purple Moon's exotic file formats easier. Eventually, the C
implementation will become Luna Purpura's reference code.


## Actually playing games

Currently, we're targeting [LÖVE][love] as the means to creating actually
playable games. This means coming up with C code to decode the game files,
and then devising Lua bindings so that a LÖVE application can access them.

LÖVE is merely an implementation detail for rapid prototyping. A more
sophisticated, portable and standalone implementation -- written in C and
likely based on SDL -- will eventually come around. But in any case, we
expect that scripts will be used in some capacity, no matter what.


## License

Luna Purpura is released under a highly permissive 2-clause BSD-style
license. Take a look at the LICENSE document for more details.


## Links

- [Purple Moon on Wikipedia][Purple Moon]
- [Purple Moon on Fandom.com][purple-moon-fandom]


[![forthebadge](https://forthebadge.com/images/badges/built-by-codebabes.svg)](https://forthebadge.com)

[love]: https://love2d.org
[Purple Moon]: https://en.wikipedia.org/wiki/Purple_Moon
[purple-moon-fandom]: https://purplemoon.fandom.com/wiki/Purple_Moon_Wiki
