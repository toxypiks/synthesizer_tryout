cmake_minimum_required(VERSION 3.16)
project(synthesizer_tryout)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SYNTHESIZER_TRYOUT
  ${CMAKE_CURRENT_SOURCE_DIR}/src/synth.c
  )

find_package(raylib)

add_executable(synthesizer_tryout)
target_sources(synthesizer_tryout
 PRIVATE
  ${SYNTHESIZER_TRYOUT}
  )

target_include_directories(synthesizer_tryout
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )

target_link_libraries(synthesizer_tryout
  ${raylib_LIBRARIES}
  )

target_link_libraries(synthesizer_tryout m)
