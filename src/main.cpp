#include "GPIO.h"

int main(){
  using namespace Arduino; 
  GPIO<13>::setOuput();
  GPIO<13>::write(0);
  return 0;
}

