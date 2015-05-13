#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"

using namespace Arduino;

int main(int argc, char *argv[]){

  GPIO<13>::setOuput();
  GPIO<13>::write(Low);
  GPIO<5,6>::setAllOutput();
  //Timer Test (Doesn't seem to work...)
  Timer0::turnOn();
  Timer0::turnOffIntr();
  Timer0::setCount(0);
  Timer0::setCompareA(0x00);
  Timer0::setCompareA(0xf0);
  Timer0::Oc0aDdr::write(Output);
  Timer0::setup(OC::Toggle, OC::Toggle, WGM::CTC, CS::Clk);

  bool value = 0;
  while(1){
    if(Timer0::countedOver()){
      value ^= 1;
      GPIO<13>::write(value);
    }
  }
  
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
