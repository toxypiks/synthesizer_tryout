#include <stdio.h>
#include <raylib.h>
#include <math.h>

#define NUM_OSCILLATORS 8

// Plan:
// - klauen buffer generierung
//  -> for schleife wollen smaples generieren
// wir geben an wo wir gerade sind t1 t2 (angangszeit bis Endzeit)
// t1,t2 sample 0 499 , 500 999, 1000 1499, .....
// synth muss sich zustand im ineren merken /\/\
// muss da anfangen wo er aufgehört hat
// void signal_generator(t1,t2, buffer)

typedef struct Oscillator{
  double freq;
  double amplitude;
} Oscillator;

void zeroBuffer(float* buffer, size_t size)
{
  for (size_t t = 0; t < size; ++t)
  {
    buffer[t] = 0.0f;
  }
}

double sine_wave(Oscillator* osc, size_t t){
  //  a * sin(f*t)
  // fmod(..., 2*pi) is modulo 2*Pi -> damit so wir immer präzise,
  double step = (double)t/48000.0;
  double wtf = osc->amplitude * sinf(fmod(2*PI*osc->freq * step , 2*PI));
  return wtf;
}

void accumulate_signal(float* buffer, Oscillator* osc, size_t t1, size_t t2){
  for (size_t t = 0; t < t2-t1; ++t){
    buffer[t] += (float)sine_wave(osc, t + t1 );
  }
}

void signal_generator(size_t t1, size_t t2, float* buffer){
  size_t buffer_length = t2-t1;
  zeroBuffer(buffer, buffer_length);

  Oscillator osc[NUM_OSCILLATORS] = {0};
  float base_freq = 25.0f + 400;

  for (size_t i = 0; i < NUM_OSCILLATORS; ++i){
    osc[i].freq = base_freq * (i + 1); //1,3,5...
    osc[i].amplitude = 1.0f / (NUM_OSCILLATORS * (i + 1));
    accumulate_signal(buffer, &osc[i], t1, t2);
  }
}



int main(void)
{
  float buffer[1024];
  signal_generator(0, 1024, buffer);
  // buffer hat jetzt werte

  for (size_t i = 0; i < 1024; ++i) {
    printf("%f\n", buffer[i]);
  }
  return 0;
}


