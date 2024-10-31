cmake_minimum_required(VERSION 3.16)
project(synthesizer_tryout)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(ADDITIVE_SYNTHESIS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/additive_synthesis.c
  )

set(ADDITIVE_SYNTHESIS_LFO
  ${CMAKE_CURRENT_SOURCE_DIR}/src/additive_synthesis_lfo.c
  )

find_package(raylib)

add_executable(additive_synthesis)
target_sources(additive_synthesis
 PRIVATE
  ${ADDITIVE_SYNTHESIS}
  )

add_executable(additive_synthesis_lfo)
target_sources(additive_synthesis_lfo
 PRIVATE
  ${ADDITIVE_SYNTHESIS_LFO}
  )

target_include_directories(additive_synthesis
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )

target_include_directories(additive_synthesis_lfo
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )

target_link_libraries(additive_synthesis
  ${raylib_LIBRARIES}
  )

target_link_libraries(additive_synthesis_lfo
  ${raylib_LIBRARIES}
  )

target_link_libraries(additive_synthesis m)

target_link_libraries(additive_synthesis_lfo m)
