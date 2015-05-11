#include "GPIO.h"

using namespace Arduino;

int main(int argc, char *argv[]){

  GPIO<10,11,12,13>::setType(0,0,1,1);

  LowLevel::Bit a = 0;
  LowLevel::Bit b = 0;
  GPIO<10,11>::read(a, b);

  GPIO<12,13>::write(a, b);

  GPIO<9>::write(1);
  while(1);
  return 0;
}
