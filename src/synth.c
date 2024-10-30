#include<stdio.h>
#include <raylib.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define STREAM_BUFFER_SIZE 1024

typedef struct {
  float phase;
  float phase_stride;
} Oscillator;

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

  float frequency = 5.0f;
  float sample_duration = (1.0f / SAMPLE_RATE);
  Oscillator osc = {.phase = 0.0f, .phase_stride = frequency * sample_duration};
  // LFO: low frequency oscillator
  Oscillator lfo = {.phase = 0.0f, .phase_stride = 1000.0f * sample_duration};
  float signal[STREAM_BUFFER_SIZE];

  while(!WindowShouldClose())
  {
	Vector2 mouse_pos = GetMousePosition();
	if (IsAudioStreamProcessed(synth_stream))
	{
	    updateOsc(&lfo);
	    frequency = 220.0f + (mouse_pos.x /(float)screen_width) * 100.0f;
	    osc.phase_stride = frequency * sample_duration;

	    updateSignal(signal, &osc);
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
