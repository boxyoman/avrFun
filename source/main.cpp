#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"

using namespace Arduino;

int main(int argc, char *argv[]){

  GPIO<13>::setOuput();
  Timer0::turnOn();
  Timer0::setCompareA(0xf0);
  Timer0::setCompareB(0x40);
  GPIO<5,6>::setAllOutput();
  Timer0::setup(OC::Clear, OC::Normal, WGM::Fast, CS::Clk);

  //bool value = 0;
  //while(1){
    //if(Timer0::didMatchA()){
      //GPIO<13>::write(value ^= 1);
    //}
  //}

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
