cmake_minimum_required(VERSION 3.0)

add_executable(xpkview
  xpkview.c
)

target_include_directories(xpkview PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../xpk
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(xpkview lpxpk lpclu lputil)

if(${LUNAPURPURA_PROVIDED_SDL})
  execute_process(
    COMMAND ${SDL2_CONFIG} --prefix=${SDL2_CONFIG_PREFIX} --cflags
    OUTPUT_VARIABLE SDL2_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  execute_process(
    COMMAND ${SDL2_CONFIG} --prefix=${SDL2_CONFIG_PREFIX} --libs
    OUTPUT_VARIABLE SDL2_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  #target_compile_options(xpkview PUBLIC ${SDL2_CFLAGS})
  # BARF -- quote marks taint the output of the first execute_process() for some unknown reason 
  target_compile_options(xpkview PUBLIC -I/Users/charlotte/devel/lunapurpura/mingw-installroot/include/SDL2 -Dmain=SDL_main)
  target_link_libraries(xpkview ${SDL2_LIBS})
else()
  target_include_directories(xpkview SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS})
  target_link_libraries(xpkview ${SDL2_LDFLAGS})
endif()
