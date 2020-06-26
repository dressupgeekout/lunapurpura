cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

add_library(lpprx STATIC
  prx.c
  prx.h
  prxindexentry.c
  prxindexentry.h
  prxmember.c
  prxmember.h
)

add_executable(prxread
  prxreader.c
  prxreader.h
  prxreadmain.c
)

target_link_libraries(prxread lpprx lputil)
