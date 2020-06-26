#!/bin/sh

set -e

build_dir=./build

if [ "$1" = "clean" ]; then
  echo "rm -rf ${build_dir}"
  rm -rf ${build_dir}
  exit $?
fi

mkdir -p ${build_dir}

pushd ${build_dir}
  cmake .. \
    -DLUNAPURPURA_BUILD_LUA_BINDINGS=OFF \
    -DLUNAPURPURA_BUILD_MRUBY_BINDINGS=OFF

  make "$@"

  for lib in clu prd xpk; do
    cp -v ./src/lua/liblua${lib}.dylib ../love/lua${lib}.so
  done
popd
