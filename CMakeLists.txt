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

set(SIMPLE_JACK_SYNTH
  ${CMAKE_CURRENT_SOURCE_DIR}/src/simple_jack_synth.c
  )

set(ADDITIVE_SYNTH_WITH_JACK
  ${CMAKE_CURRENT_SOURCE_DIR}/src/additive_synth_with_jack.c
  )

set(OSCILLATOR_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/oscillator.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/oscillator.h
  )

set(RAY_OUT_BUFFER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ray_out_buffer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ray_out_buffer.h
  )

set(JACK_STUFF_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/jack_stuff.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/jack_stuff.h
  )

find_package(raylib)

find_package(Jack REQUIRED)

add_executable(additive_synthesis)
target_sources(additive_synthesis
 PRIVATE
  ${ADDITIVE_SYNTHESIS}
  )
target_include_directories(additive_synthesis
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )
target_link_libraries(additive_synthesis
  ${raylib_LIBRARIES}
  m
  )

add_executable(additive_synthesis_lfo)
target_sources(additive_synthesis_lfo
 PRIVATE
  ${ADDITIVE_SYNTHESIS_LFO}
  )
target_include_directories(additive_synthesis_lfo
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )
target_link_libraries(additive_synthesis_lfo
  ${raylib_LIBRARIES}
  m
  )

add_executable(sine_wave_generator)
target_sources(sine_wave_generator
 PRIVATE
  ${SINE_WAVE_GENERATOR}
  )
target_include_directories(sine_wave_generator
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )
target_link_libraries(sine_wave_generator
  ${raylib_LIBRARIES}
  m
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
target_include_directories(jack_client
  PUBLIC ${JACK_INCLUDE_DIRS}
)
target_link_libraries(jack_client
  ${JACK_LIBRARIES}
)

add_executable(simple_jack_synth)
target_sources(simple_jack_synth
 PRIVATE
  ${SIMPLE_JACK_SYNTH}
  )
target_include_directories(simple_jack_synth
  PUBLIC ${JACK_INCLUDE_DIRS}
)
target_link_libraries(simple_jack_synth
  ${JACK_LIBRARIES}
  m
)

add_library(oscillator)
target_sources(oscillator
 PRIVATE
  ${OSCILLATOR_SOURCES}
  )
target_include_directories(oscillator
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )
target_link_libraries(oscillator
  ${raylib_LIBRARIES}
  m
  )

add_library(ray_out_buffer)
target_sources(ray_out_buffer
 PRIVATE
  ${RAY_OUT_BUFFER_SOURCES}
  )
target_include_directories(ray_out_buffer
 PRIVATE
 ${INCLUDE}
 )

add_library(jack_stuff)
target_sources(jack_stuff
 PRIVATE
  ${JACK_STUFF_SOURCES}
  )
target_include_directories(jack_stuff
 PRIVATE
 ${INCLUDE}
 )

target_link_libraries(jack_stuff
  ${JACK_LIBRARIES}
  )

add_executable(additive_synth_with_jack)
target_sources(additive_synth_with_jack
 PRIVATE
  ${ADDITIVE_SYNTH_WITH_JACK}
  )
target_include_directories(additive_synth_with_jack
 PRIVATE
 ${raylib_INCLUDE_DIRS}
 ${JACK_INCLUDE_DIRS}
 )
target_link_libraries(additive_synth_with_jack
  ${raylib_LIBRARIES}
  ${JACK_LIBRARIES}
  m
  oscillator
  ray_out_buffer
  jack_stuff
  )
