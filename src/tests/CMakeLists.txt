cmake_minimum_required(VERSION 3.0)

add_executable(clu_test clu_test.c)

target_include_directories(clu_test PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../prx
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(clu_test lpclu lpprx lputil)
