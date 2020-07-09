cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

# User-settable options.
set(LUNAPURPURA_DEBUG ON CACHE BOOL "Enable debug output")
set(LUNAPURPURA_BUILD_LUA_BINDINGS OFF CACHE BOOL "Build the Lua/LuaJIT bindings")
set(LUNAPURPURA_BUILD_MRUBY_BINDINGS OFF CACHE BOOL "Build the mruby bindings")
set(LUNAPURPURA_PNG_SUPPORT ON CACHE BOOL "Enable conversion to PNG")

if(${LUNAPURPURA_BUILD_LUA_BINDINGS})
  message(STATUS "Lua bindings enabled")
  set(LUA_INCLUDEDIR "${PROJECT_SOURCE_DIR}/lua/include" CACHE PATH "Location of Lua 5.1/LuaJIT's header files")
  set(LUA_LIBDIR "" CACHE PATH "Location of Lua 5.1/LuaJIT's library")
endif()

if(${LUNAPURPURA_BUILD_MRUBY_BINDINGS})
  message(STATUS "mruby bindings enabled")
  set(MRUBY_INCLUDEDIR "" CACHE PATH "Location of mruby's header files")
  set(MRUBY_LIBDIR "" CACHE PATH "Location of mruby's library")
endif()

if(${LUNAPURPURA_PNG_SUPPORT})
  message(STATUS "PNG support enabled")
  find_package(PNG REQUIRED)
endif()

include(CheckFunctionExists)
check_function_exists(getprogname LUNAPURPURA_HAVE_GETPROGNAME)

if(${LUNAPURPURA_HAVE_GETPROGNAME})
  add_definitions(-DLUNAPURPURA_HAVE_GETPROGNAME)
endif()

# Global build parameters.
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall)

if(${LUNAPURPURA_DEBUG})
  message(STATUS "DEBUG macro enabled")
  add_definitions(-DLUNAPURPURA_DEBUG)
endif()

# Good to go.
add_subdirectory(src)
