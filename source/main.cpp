#include "GPIO.h"

using namespace Arduino;

int main(int argc, char *argv[]){

  GPIO<10,11,12,13>::setType(0,0,1,1);

  uint8_t a = 0;
  uint8_t b = 0;
  GPIO<10,11>::read(a, b);

  GPIO<12,13>::write(a, b);

  while(1);
  return 0;
}
