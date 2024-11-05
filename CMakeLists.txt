cmake_minimum_required(VERSION 3.16)
project(synthesizer_tryout)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

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

set(PLAY_WAVE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/play_wave.c
  )

set(JACK_CLIENT
  ${CMAKE_CURRENT_SOURCE_DIR}/src/jack_client.c
  )

find_package(raylib)

find_package(Jack REQUIRED)

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

add_executable(play_wave)
target_sources(play_wave
 PRIVATE
  ${PLAY_WAVE}
  )

add_executable(jack_client)
target_sources(jack_client
 PRIVATE
  ${JACK_CLIENT}
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

target_include_directories(jack_client
  PUBLIC ${JACK_INCLUDE_DIRS}
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

target_link_libraries(jack_client
  ${JACK_LIBRARIES}
)

target_link_libraries(additive_synthesis m)

target_link_libraries(additive_synthesis_lfo m)

target_link_libraries(sine_wave_generator m)
