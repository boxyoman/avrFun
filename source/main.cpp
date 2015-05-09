#include "GPIO.h"

using namespace Arduino;

int main(int argc, char *argv[]){
  GPIO<12, 13>::setOutput();
  GPIO<12,13>::write(1, 0);

  while(1);
  return 0;
}
