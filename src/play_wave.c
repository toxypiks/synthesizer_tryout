#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  //einlesen des ersten Werts => Anzahl von allen Werten = nvalues
  //erstell mir ein Array mit malloc mit nvalues aus float werten
  size_t nvalues = 0;
  scanf("%d", &nvalues);

  double* values  = (double*)malloc(nvalues * sizeof(double));

  for (size_t i = 0; i < nvalues; ++i) {
    scanf("%lf", &values[i]);
  }

  for(size_t i = 0; i < nvalues; ++i) {
	printf("%f\n", values[i]);
  }

  free(values);
  return 0;
}
