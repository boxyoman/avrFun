#include "GPIO.h"
#include "Analog.h"

using namespace Arduino;

int main(int argc, char *argv[]){

  //Digital Test
  //GPIO<10,11,12,13>::setType(0,0,1,1);
  //unsigned int a = 0;
  //unsigned int b = 0;
  //GPIO<10,11>::read(a, b);
  //GPIO<12,13>::write(a, b);
  
  Analog<0>::init();
  unsigned int value = Analog<0>::read();
  if(value > 500){
    GPIO<13>::write(Low);
  }else{
    GPIO<13>::write(High);
  }

  while(1);
  return 0;
}
