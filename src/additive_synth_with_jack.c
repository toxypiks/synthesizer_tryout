#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <jack/ringbuffer.h>
#include <unistd.h>
#include <jack/jack.h>

typedef struct Osc {
  float freq;
  float time_step;
} Osc;

typedef struct JackStuff {
  jack_port_t* output_port;
  jack_ringbuffer_t* ringbuffer_audio;
  jack_ringbuffer_t* ringbuffer_video;
} JackStuff;


void gen_signal_in_buf(float* buf, size_t buf_length, Osc* osc) {
  for(size_t i = 0; i < buf_length; ++i) {
	buf[i] = sinf(2*PI*osc->freq*(osc->time_step/48000.0));
	osc->time_step += 1;
  }
}

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
  jack_activate(client);

  float my_data_buf[1024];
  Osc my_osci = {.freq = 440, .time_step = 0};

  // create signal
  // has to run in own thread TODO
  // play 10 sec

  const int screen_width = 1024;
  const int screen_height = 768;
  InitWindow(screen_width, screen_height, "synthesizer_tryout");
  SetTargetFPS(60);

  float raylib_data_buf[1024];

  size_t num_bytes = jack_ringbuffer_read_space (jack_stuff.ringbuffer_audio);

  while(!WindowShouldClose() && num_bytes < 96000 * sizeof(float)) {

	  gen_signal_in_buf(my_data_buf, 1024, &my_osci);
	  jack_ringbuffer_write(jack_stuff.ringbuffer_audio, (void *) my_data_buf, 1024*sizeof(float));
	  jack_ringbuffer_write(jack_stuff.ringbuffer_video, (void *) my_data_buf, 1024*sizeof(float));
      //from ringbuffer to raylib_data_buf TODO

	  BeginDrawing();
	  ClearBackground(BLACK);
	  EndDrawing();
  }
  CloseWindow();

  jack_deactivate(client);
  jack_client_close(client);
  jack_ringbuffer_free(jack_stuff.ringbuffer_audio);

  return 0;
}
