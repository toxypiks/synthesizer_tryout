#include <stdio.h>
#include <raylib.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define SAMPLE_DURATION (1.0f / SAMPLE_RATE)
#define STREAM_BUFFER_SIZE 1024
#define NUM_OSCILLATORS 16

typedef struct {
  float phase;
  float freq;
  float omega_freq_modulated;
  float amplitude;
  float time_step;
  float old_time_step;
} Oscillator;


void resetOsc(Oscillator* osc) {
  osc->time_step = osc->old_time_step;
}

void save_time_stepOsc(Oscillator* osc) {
  osc->old_time_step = osc->time_step;
}

void updateOsc(Oscillator* osc, float lfo_freq_modulation)
{
  // doing frequenz modulation
  // sin((w + (lfo * w)) * t)
  float omega = 2.0f * PI * osc->freq;
  osc->omega_freq_modulated = (omega + lfo_freq_modulation * omega);
  // zeit hochzählen
  osc->time_step += 1.0/SAMPLE_RATE;
}

// STREAM_BUFFER_SIZE anzahl der samples im
// wieviel zeit pro abtastwert
//samplerate = 44100/s
// STREAM_BUFFER_SIZE/44100 => wieviel zeit streambuffersize

void zeroSignal(float* signal)
{
   for (size_t t = 0; t < STREAM_BUFFER_SIZE; ++t)
	  {
	    signal[t] = 0.0f;
	  }
}

float sineWaveOsc(Oscillator* osc)
{
  // sin(2*pi*t*f + phase)
  // normalisierte frequenz
  float omega = osc->omega_freq_modulated;
  return sinf( fmod((osc->time_step * omega + osc->phase/omega), 2*PI)) * osc->amplitude;
}

void accumulateSignal(float* signal, Oscillator* osc, float* signal_lfo)
{
	for (size_t t = 0; t < STREAM_BUFFER_SIZE; ++t)
	{
		updateOsc(osc, 0.001f * signal_lfo[t]);
	    signal[t] += sineWaveOsc(osc);
	}
}

void calc_lfo_signal(float* signal_lfo, Oscillator* lfo)
{
	for (size_t t = 0; t < STREAM_BUFFER_SIZE; ++t)
	{
		updateOsc(lfo, 0.0);
	    signal_lfo[t] = sineWaveOsc(lfo);
	}
}

int main(void)
{
  const int screen_width = 1024;
  const int screen_height = 768;
  InitWindow(screen_width, screen_height, "synthesizer_tryout");
  SetTargetFPS(60);
  InitAudioDevice();

  unsigned int sample_rate = SAMPLE_RATE;
  SetAudioStreamBufferSizeDefault(STREAM_BUFFER_SIZE);
  AudioStream synth_stream = LoadAudioStream(sample_rate, sizeof(float) * 8, 1);

  SetAudioStreamVolume(synth_stream, 0.05f);
  PlayAudioStream(synth_stream);

  Oscillator osc[NUM_OSCILLATORS] = {0};
  Oscillator lfo = {.phase = 0.0f};
  lfo.freq = 3.0f;
  lfo.amplitude = 50.0f;
  float signal[STREAM_BUFFER_SIZE];
  float signal_lfo[STREAM_BUFFER_SIZE];

  while(!WindowShouldClose())
  {
	Vector2 mouse_pos = GetMousePosition();
	float normalized_mouse_x = (mouse_pos.x /(float)screen_width);
	float base_freq = 25.0f + 400; //(normalized_mouse_x * 400.0f);

	calc_lfo_signal(signal_lfo, &lfo);
	if (IsAudioStreamProcessed(synth_stream))
	{
		zeroSignal(signal);
		for (size_t i = 0; i < NUM_OSCILLATORS; ++i)
		{
		  if (i % 2 != 0)
		  {
		    osc[i].freq = base_freq * i;
			osc[i].amplitude = 1.0f / NUM_OSCILLATORS;
		    accumulateSignal(signal, &osc[i], signal_lfo);
			//printf("osc signal %f\n", signal[i]);
			//printf("lfosc signal %f\n", lfo.freq);
		  }
		}
	    UpdateAudioStream(synth_stream, signal, STREAM_BUFFER_SIZE);
	}
	printf("osc freq: %f\n", osc[1].freq);
    printf("osc mod freq: %f\n", osc[1].omega_freq_modulated);

	BeginDrawing();
	ClearBackground(BLACK);
	for (size_t i = 0; i < screen_width; ++i)
	{
	  DrawPixel(i, screen_height/2 + (int)(signal[i] * 100), BLUE);
	}
	EndDrawing();
  }
  UnloadAudioStream(synth_stream);
  CloseAudioDevice();
  CloseWindow();
}
