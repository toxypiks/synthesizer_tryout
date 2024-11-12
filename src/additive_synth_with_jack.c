#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <jack/ringbuffer.h>
#include <unistd.h>
#include <jack/jack.h>
#include <stdlib.h>
#include <string.h>
#include "oscillator.h"
#include "ray_out_buffer.h"

typedef struct JackStuff {
  jack_port_t* output_port;
  jack_ringbuffer_t* ringbuffer_audio;
  jack_ringbuffer_t* ringbuffer_video;
} JackStuff;

// View Teil 2
// Sound ausgabe
int process(jack_nframes_t nframes, void* jack_stuff_raw)
{
  JackStuff* jack_stuff = (JackStuff*)jack_stuff_raw;
  float* output_buffer= (float*)jack_port_get_buffer (jack_stuff->output_port, nframes);

  if(jack_stuff->ringbuffer_audio){ // is buffer even there?
    // see if theres enough data in buffer to read nframes out of it (num_bytes >nframes)
	size_t num_bytes = jack_ringbuffer_read_space (jack_stuff->ringbuffer_audio);
	if(num_bytes >= (nframes* sizeof(float))) {
	  jack_ringbuffer_read(jack_stuff->ringbuffer_audio, (char*)output_buffer, nframes * sizeof(float));
	} else {
	  for ( int i = 0; i < (int) nframes; i++)
	  {
        output_buffer[i] = 0.0;
	  }
	}
  }
  return 0;
}


int main(void) {

  JackStuff jack_stuff = {.output_port = 0, .ringbuffer_audio = NULL, .ringbuffer_video = NULL};

  jack_client_t* client = jack_client_open ("JackClientAdditiveSynth",
                                            JackNullOption,
                                            0,
                                            0 );

  jack_stuff.output_port = jack_port_register (client,
                                    "output",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput,
                                    0 );

  size_t my_size = 192000 * sizeof(float);
  jack_stuff.ringbuffer_audio = jack_ringbuffer_create(my_size);
  jack_stuff.ringbuffer_video = jack_ringbuffer_create(my_size);

  jack_set_process_callback(client, process ,(void*)&jack_stuff);
  //client.set_sample_rate(48000);
  jack_activate(client);

  float my_data_buf[1024];
  Oscillator my_osci = {.freq = 440, .time_step = 0};

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
	  size_t num_bytes = jack_ringbuffer_read_space (jack_stuff.ringbuffer_audio);

	  if(num_bytes < 96000 * sizeof(float)) {
		gen_signal_in_buf(&my_osci, my_data_buf, 1024);
		change_time_step(&my_osci,1024);
		jack_ringbuffer_write(jack_stuff.ringbuffer_video, (void *) my_data_buf, 1024*sizeof(float));
		jack_ringbuffer_write(jack_stuff.ringbuffer_audio, (void *) my_data_buf, 1024*sizeof(float));
	  }
	}

	// checken sind daten im ringbuffer_video?
	if(jack_stuff.ringbuffer_video){
	  float output_buffer[1024];
	  size_t num_bytes = jack_ringbuffer_read_space (jack_stuff.ringbuffer_video);
	  if(num_bytes >= (1024* sizeof(float))) {
		jack_ringbuffer_read(jack_stuff.ringbuffer_video, (char*)output_buffer, 1024 * sizeof(float));
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

  jack_deactivate(client);
  jack_client_close(client);
  jack_ringbuffer_free(jack_stuff.ringbuffer_audio);

  return 0;
}
