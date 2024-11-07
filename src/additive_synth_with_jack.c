#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <jack/ringbuffer.h>
#include <unistd.h>
#include <jack/jack.h>

jack_port_t* outputPort = 0;
jack_ringbuffer_t *rb = NULL;

typedef struct {
  float freq;
  float time_step;
} Osc;


void gen_signal_in_buf(float* buf, size_t buf_length, Osc* osc) {
  for(size_t i = 0; i < buf_length; ++i) {
	buf[i] = sinf(2*PI*osc->freq*(osc->time_step/48000.0));
	osc->time_step += 1;
  }
}

int process(jack_nframes_t nframes, void* )
{
  float* outputBuffer= (float*)jack_port_get_buffer ( outputPort, nframes);

  // aus dem Ringbuffer daten in out schreiben
  if(rb){ // gibts den Ringbuffer überhaupt?
    // wollen nframes an daten haben (? >nframes daten im Buffer)
	size_t num_bytes = jack_ringbuffer_read_space (rb);
	if(num_bytes > (nframes* sizeof(float))) {
	  jack_ringbuffer_read(rb, (char*)outputBuffer, nframes * sizeof(float));
	} else {
	  for ( int i = 0; i < (int) nframes; i++)
	  {
        outputBuffer[i] = 0.0;
	  }
	}
  }
  return 0;
}


int main(void) {

  jack_client_t* client = jack_client_open ("JackClientAdditiveSynth",
                                            JackNullOption,
                                            0,
                                            0 );
  jack_set_process_callback  (client, process , 0);

  outputPort = jack_port_register ( client,
                                    "output",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput,
                                    0 );
  size_t my_size = 192000 * sizeof(float);
  rb = jack_ringbuffer_create (my_size);

  jack_activate(client);

  float my_data_buf[1024];

  Osc my_osci = {.freq = 440, .time_step = 0};

  // hier signalerzeugung
  // nächste mal in eigenen thread
  // 10 sekunden ausgeben
  size_t counter = 0;
  while(counter < 48000 * 10 ){
	//häppchenweise daten in buffer
	// bei jeden Happpen checken wieviel schon drin
	// wenn da schon 2sekunden drin sind -> sleep(1)
	size_t num_bytes = jack_ringbuffer_read_space (rb);
	if (num_bytes < 96000 * sizeof(float)) {
	  gen_signal_in_buf(my_data_buf, 1024, &my_osci);
	  jack_ringbuffer_write (rb, (void *) my_data_buf, 1024*sizeof(float));
	  counter += 1024;
	} else {
	  sleep(1);
	}
  }

  jack_deactivate(client);
  jack_client_close(client);
  jack_ringbuffer_free(rb);
  return 0;
}
