cmake_minimum_required(VERSION 3.0)

add_subdirectory(clu)
add_subdirectory(dib)
add_subdirectory(lputil)
add_subdirectory(matrix)
add_subdirectory(mmfw)
add_subdirectory(prx)
add_subdirectory(smf)
add_subdirectory(xmv)
add_subdirectory(xpk)

if(${LUNAPURPURA_BUILD_LUA_BINDINGS})
  add_subdirectory(lua)
endif()

if(${LUNAPURPURA_BUILD_MRUBY_BINDINGS})
  add_subdirectory(mruby)
endif()

if(${LUNAPURPURA_BUILD_TESTS})
  add_subdirectory(tests)
endif()
