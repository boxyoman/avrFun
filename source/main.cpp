#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"

using namespace Arduino;

int main(int argc, char *argv[]){

  GPIO<13>::setOuput();
  GPIO<13>::write(Low);
  //Timer Test (Doesn't seem to work...)
  Timer0::turnOn();
  Timer0::setCompareA(0x40);
  Timer0::setCompareB(0x40);
  GPIO<5,6>::setAllOutput();
  Timer0::setup(OC::Set, OC::Clear, WGM::Fast, CS::Clk1024);

  //Analog<0>::init();
  //unsigned int value = Analog<0>::read();
  //if(value > 500){
    //GPIO<13>::write(Low);
  //}else{
    //GPIO<13>::write(High);
  //}

  while(1);
  return 0;
}
