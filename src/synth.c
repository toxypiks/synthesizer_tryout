#include<stdio.h>
#include <raylib.h>
#include <math.h>

typedef struct {
  float phase;
} Oscillator;

int main() {
  const int screen_width = 1024;
  const int screen_height = 768;
  InitWindow(screen_width, screen_height, "synthesizer_tryout");

  SetTargetFPS(60);

  Oscillator osc = {.phase = 0.0f};
  float signal[screen_width];

  for (size_t i = 0; i < screen_width; ++i)
	{
	  signal[i] = sinf((float)i *0.1f);
	}

  while(!WindowShouldClose())
  {
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
