
#include <iostream>
#include <cassert>

#include <softinj.hh>

#define ITER_LIMIT 1000

/* Note: the type of the 'sum' and 'i' variables can be either
   char, short, int, long, float, or double.
   However, when type is float or double the bitwise logic operations
   can not be used.
   Moreover, mixed type wrappers are not support. 
*/

int
original()
{
  int sum = 0;
  int i;

  for (i=0; i<ITER_LIMIT; i++) {
    sum = sum + i;
  }

  sum = sum - i;
  sum = sum ^ i;
  sum = sum | i;
  sum = sum & i;


  return sum;
}

int
modified()
{
  int sum = 0;
  int i;

  for (i=0; i<ITER_LIMIT; i++) {
    sum = softinj::ADD(sum, i);
  }

  sum = softinj::SUB(sum, i);
  sum = softinj::XOR(sum, i);
  sum = softinj::OR(sum, i);
  sum = softinj::AND(sum, i);


  return sum;
}

int
main(int argc, char **argv)
{

  // The initialization function of the library needs to be
  // before any use of the wrapping functions
  softinj::initialize(SOFTINJ_STATUS,
		      SOFTINJ_VOLTAGE,
		      SOFTINJ_START_BIT, SOFTINJ_END_BIT,
		      SOFTINJ_RANDOM_SEED,
		      SOFTINJ_DEBUG_LEVEL);

  
  int o = original();
  int m = modified();
    
  printf("original: %d, modified: %d\n", o, m);

  // The call on the finalize function of the library is not
  // essential for the use of the library. However, it may be
  // useful for recording the statistics collected during the
  // execution of the application
  // finalize() prints in stderr
  softinj::finalize();
}

