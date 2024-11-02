#include <stdio.h>
#include <raylib.h>

#include <stdlib.h>
#include <math.h>
#define _USE_MATH_DEFINES

int main(int argc, char *argv[]) {
  unsigned int freq  = atoi(argv[1]);
  float  duration = atof(argv[2]);
  const unsigned int sample_rate = 48000;
  unsigned int end = (sample_rate * (int)duration);
  unsigned int n = 0;

  float s = 0;
  printf("%d\n", end);
  while(n < end) {
    s =  sin((2 * PI * freq * n) / sample_rate);
    printf("%f\n", s);
    n++;
  }
  return 0;
}
