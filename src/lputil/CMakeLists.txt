cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

add_library(lputil STATIC
  lpstatus.c
  lpstatus.h
  lputil.c
  lputil.h
)
