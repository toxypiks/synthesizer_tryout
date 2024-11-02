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

set(SINE_WAVE_GENERATOR
  ${CMAKE_CURRENT_SOURCE_DIR}/src/sine_wave_generator.c
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

add_executable(sine_wave_generator)
target_sources(sine_wave_generator
 PRIVATE
  ${SINE_WAVE_GENERATOR}
  )

target_include_directories(additive_synthesis
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )

target_include_directories(additive_synthesis_lfo
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )

target_include_directories(sine_wave_generator
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )

target_link_libraries(additive_synthesis
  ${raylib_LIBRARIES}
  )

target_link_libraries(additive_synthesis_lfo
  ${raylib_LIBRARIES}
  )

target_link_libraries(sine_wave_generator
  ${raylib_LIBRARIES}
  )

target_link_libraries(additive_synthesis m)

target_link_libraries(additive_synthesis_lfo m)

target_link_libraries(sine_wave_generator m)
