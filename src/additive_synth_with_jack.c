#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <jack/ringbuffer.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "oscillator.h"
#include "ray_out_buffer.h"
#include "jack_stuff.h"

int main(void) {
  JackStuff* jack_stuff = create_jack_stuff("JackClientAdditiveSynth",192000);
  float my_data_buf[1024];
  Oscillator my_osci = {.freq = 440, .time_step = 0};

  Oscillator osc[8] = {0};
  // create signal
  // has to run in own thread TODO
  // play 10 sec

  const int screen_width = 1024;
  const int screen_height = 768;
  InitWindow(screen_width, screen_height, "synthesizer_tryout");
  SetTargetFPS(60);

  float raylib_data_buf[1024];

  size_t old_buffer_level = 0;
  size_t old_num_jack_data = 0;

  RayOutBuffer ray_out_buffer = create_ray_out_buffer(10000);
  int h = GetRenderHeight();
  while(!WindowShouldClose()) {

	Vector2 mouse_pos = GetMousePosition();
    float normalized_mouse_x = (mouse_pos.x /(float)screen_width);
	float new_freq = 50.0f + (normalized_mouse_x * 1000.0f);

    // hier beginnt Model
	{
	  change_frequency(&my_osci, new_freq);
       // Audio erzeugung
	  // später eigentich in eigenen thread ->tonerzeugugsthread
	  size_t num_bytes = jack_ringbuffer_read_space (jack_stuff->ringbuffer_audio);

	  if(num_bytes < 96000 * sizeof(float)) {

		for(size_t i = 1; i <= 8; ++i) {
		  if(i % 2 == 0) {
			change_frequency(&osc[i], (new_freq *i));
		    gen_signal_in_buf(&osc[i], my_data_buf, 1024);
		    change_time_step(&osc[i],1024);
		    jack_ringbuffer_write(jack_stuff->ringbuffer_video, (void *) my_data_buf, 1024*sizeof(float));
		    jack_ringbuffer_write(jack_stuff->ringbuffer_audio, (void *) my_data_buf, 1024*sizeof(float));
		  }
		}
	  }
	}

	// checken sind daten im ringbuffer_video?
	if(jack_stuff->ringbuffer_video){
	  float output_buffer[1024];
	  size_t num_bytes = jack_ringbuffer_read_space (jack_stuff->ringbuffer_video);
	  if(num_bytes >= (1024* sizeof(float))) {
		jack_ringbuffer_read(jack_stuff->ringbuffer_video, (char*)output_buffer, 1024 * sizeof(float));
	  } else {
		for ( int i = 0; i < 1024; i++)
		{
		  output_buffer[i] = 0.0;
		}
	  }
	  copy_to_ray_out_buffer(&ray_out_buffer, output_buffer, 1024);
	}
	printf("daten in rayout buffer: %d\n",ray_out_buffer.global_frames_count);
	// View teil 1
	// daten aus Ringbuffer rausholen
	// Daten zeichnen
	BeginDrawing();
	ClearBackground(BLACK);
	float cell_width = (float)GetRenderWidth()/ray_out_buffer.global_frames_count;
	for (size_t i = 0; i < ray_out_buffer.global_frames_count; ++i) {
	  float t = ray_out_buffer.global_frames[i];
	  if (t > 0) {
	    DrawRectangle(i * cell_width, h/2 - h/2*t, 1, h/2*t, RED);
	  } else {
		DrawRectangle(i * cell_width, h/2, 1, -h/2*t, BLUE);
	  }
	}

	EndDrawing();
  }
  CloseWindow();
  jack_stuff_clear(jack_stuff);

  return 0;
}
