#!/bin/sh

set -e

build_dir=./build
love_dir=./love
lua_bindings=OFF

if [ "$1" = "clean" ]; then
  echo "rm -rf ${build_dir}"
  rm -rf ${build_dir}
  exit $?
fi

mkdir -p ${build_dir}
mkdir -p ${love_dir}

cd ${build_dir}
  cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DLUNAPURPURA_BUILD_LUA_BINDINGS=${lua_bindings} \
    -DLUNAPURPURA_BUILD_MRUBY_BINDINGS=OFF \
    -DLUNAPURPURA_PNG_SUPPORT=OFF

  make "$@"

  if [ "${lua_bindings}" != "OFF" ]; then
    for lib in clu prx xpk; do
      cp -v ./src/lua/liblua${lib}.dylib ../${love_dir}/lua${lib}.so
    done
  fi
cd -
