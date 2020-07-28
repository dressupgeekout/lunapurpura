#!/bin/sh
#
# I run this script on my Mac (other Unix-like systems probably work, too)
# in order to build binaries which run on Windows.  -dressupgeekout
#

set -e

MINGW_CC=x86_64-w64-mingw32-gcc
MINGW_WINDRES=x86_64-w64-mingw32-windres

zlib=zlib-1.2.11       # source
libpng=libpng-1.6.37   # source
sdl2=SDL2-2.0.12       # libs + headers

__here__="$(pwd)"

srcdir=${__here__}/mingw-src
builddirbase=${__here__}/mingw-build
installroot=${__here__}/mingw-installroot

########## ########## ##########

mkdir -p ${srcdir}
mkdir -p ${builddirbase}
mkdir -p ${installroot}

zlib_srcdir=${srcdir}/${zlib}
libpng_srcdir=${srcdir}/${libpng}
sdl2_dir=${srcdir}/${sdl2}/x86_64-w64-mingw32
lunapurpura_srcdir=${__here__}

zlib_builddir=${builddirbase}/${zlib}
libpng_builddir=${builddirbase}/${libpng}
lunapurpura_builddir=${builddirbase}/lunapurpura

# Obtain the source code if necessary.
test -d ${zlib_srcdir}
test -d ${libpng_srcdir}
test -d ${sdl2_dir}


# === zlib ===
mkdir -p ${zlib_builddir}
cd ${zlib_builddir}

cmake ${zlib_srcdir} \
  -DMINGW=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_INSTALL_PREFIX=${installroot} \
  -DCMAKE_C_COMPILER=${MINGW_CC} \
  -DCMAKE_RC_COMPILER=${MINGW_WINDRES}

cmake --build .
cmake --install .


# === libpng ===
mkdir -p ${libpng_builddir}

cd ${libpng_builddir}

cmake ${libpng_srcdir} \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_INSTALL_PREFIX=${installroot} \
  -DCMAKE_C_COMPILER=${MINGW_CC} \
  -DPNG_BUILD_ZLIB=ON \
  -DZLIB_LIBRARY=${installroot}/bin/libzlib.dll \
  -DZLIB_INCLUDE_DIR=${installroot}/include

cmake --build .
cmake --install .


# === SDL2 ===
#
# We have a mingw-ready prebuilt distribution of SDL2. Let's just copy it
# directly into the installroot.
cp -Rv ${sdl2_dir}/ ${installroot}


# === Luna Purpura ===
rm -r ${lunapurpura_builddir}
mkdir -p ${lunapurpura_builddir}

cd ${lunapurpura_builddir}

#sdl2_cflags="$(${installroot}/bin/sdl2-config --prefix=${installroot} --cflags)"
#sdl2_libs="$(${installroot}/bin/sdl2-config --prefix=${installroot} --libs)"

cmake ${lunapurpura_srcdir} \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_INSTALL_PREFIX=${installroot} \
  -DCMAKE_C_COMPILER=${MINGW_CC} \
  -DLUNAPURPURA_DEBUG=ON \
  -DLUNAPURPURA_BUILD_LUA_BINDINGS=OFF \
  -DLUNAPURPURA_BUILD_MRUBY_BINDINGS=OFF \
  -DLUNAPURPURA_XPK_TRACE=OFF \
  -DLUNAPURPURA_BUILD_TESTS=ON \
  -DLUNAPURPURA_PNG_SUPPORT=OFF \
  -DLUNAPURPURA_SDL_SUPPORT=ON \
  -DLUNAPURPURA_PROVIDED_SDL=ON \
  -DSDL2_CONFIG="${installroot}/bin/sdl2-config" \
  -DSDL2_CONFIG_PREFIX="${installroot}"

#cmake --build .
make VERBOSE=1
