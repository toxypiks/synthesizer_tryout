#include<stdio.h>
#include <raylib.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define STREAM_BUFFER_SIZE 1024
#define NUM_OSCILLATORS 16
#define SAMPLE_DURATION (1.0f / SAMPLE_RATE)

typedef struct {
  float phase;
  float phase_stride;
} Oscillator;

void setOscFrequency(Oscillator* osc, float frequency)
{
  osc->phase_stride = frequency * SAMPLE_DURATION;
}

void updateOsc(Oscillator* osc)
{
	osc->phase += osc->phase_stride;
	if (osc->phase >= 1.0f) osc->phase -= 1.0f;
}

void updateSignal(float* signal, Oscillator* osc)
{
   for (size_t t = 0; t < STREAM_BUFFER_SIZE; ++t)
	  {
		updateOsc(osc);
	    signal[t] = sinf(2.0f*PI * osc->phase);
	  }
}

void zeroSignal(float* signal)
{
   for (size_t t = 0; t < STREAM_BUFFER_SIZE; ++t)
	  {
	    signal[t] = 0.0f;
	  }
}

float sineWaveOsc(Oscillator* osc)
{
  return sinf(2.0f*PI * osc->phase);
}

void accumulateSignal(float* signal, Oscillator* osc, float amplitude)
{
   for (size_t t = 0; t < STREAM_BUFFER_SIZE; ++t)
	  {
		updateOsc(osc);
	    signal[t] += sineWaveOsc(osc) * amplitude;
	  }
}

int main(void)
{
  const int screen_width = 1024;
  const int screen_height = 768;
  InitWindow(screen_width, screen_height, "synthesizer_tryout");
  SetTargetFPS(60);
  InitAudioDevice();

  SetAudioStreamBufferSizeDefault(STREAM_BUFFER_SIZE);
  AudioStream synth_stream = LoadAudioStream(SAMPLE_RATE, sizeof(float)*8, 1);

  SetAudioStreamVolume(synth_stream, 0.05f);
  PlayAudioStream(synth_stream);

  float frequency = 0.0f;
  float signal[STREAM_BUFFER_SIZE];

  // LFO: low frequency oscillator
  Oscillator lfo = {.phase = 0.0f};
  setOscFrequency(&lfo, 10.0f * 1024); //wobble ten times per second
  Oscillator osc[NUM_OSCILLATORS] = {0};

  float detune = 0.1f;

  while(!WindowShouldClose())
  {
	Vector2 mouse_pos = GetMousePosition();
	float normalized_mouse_x = (mouse_pos.x /(float)screen_width);
	detune = 1.0f + normalized_mouse_x * 10.0f;
	if (IsAudioStreamProcessed(synth_stream))
	{
		zeroSignal(signal);
		updateOsc(&lfo);
		float base_freq = 25.0f + (normalized_mouse_x * 400.0f) + (sineWaveOsc(&lfo) * 50.0f);
		for (size_t i = 0; i < NUM_OSCILLATORS; ++i)
		{
		  if (i % 2 != 0)
		  {
		    frequency = base_freq * i;
		    float phase_stride = frequency * SAMPLE_DURATION;
		    osc[i].phase_stride = phase_stride;
		    accumulateSignal(signal, &osc[i], 1.0f / NUM_OSCILLATORS);
		  }
		}
	    UpdateAudioStream(synth_stream, signal, STREAM_BUFFER_SIZE);
	}

	BeginDrawing();
	ClearBackground(BLACK);
	if(IsAudioStreamPlaying(synth_stream))
	{
	    DrawText(TextFormat("Frequency: %f", frequency), 100, 100, 20, RED);
	}
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
