#include<stdio.h>
#include <raylib.h>
#include <math.h>

typedef struct {
  float phase;
} Oscillator;

void updateSignal(float* signal, float frequency, float sample_duration)
{
   for (size_t t = 0; t < 1024; ++t)
	  {
	    signal[t] = sinf(2*PI * frequency * sample_duration * (float)t);
	  }
}

int main(void)
{
  const int screen_width = 1024;
  const int screen_height = 768;
  InitWindow(screen_width, screen_height, "synthesizer_tryout");
  SetTargetFPS(60);

  Oscillator osc = {.phase = 0.0f};
  float signal[screen_width];
  float frequency = 5.0f;
  unsigned int sample_rate = 1024;
  float sample_duration = (1.0f / 1024);

  while(!WindowShouldClose())
  {
	updateSignal(&signal[0], frequency, sample_duration);
	frequency += 0.01f;
	BeginDrawing();
	ClearBackground(BLACK);
	for (size_t i = 0; i < screen_width; ++i)
	{
	  DrawPixel(i, screen_height/2 + (int)(signal[i] * 100), BLUE);
	}
	EndDrawing();
  }
  CloseWindow();
}
