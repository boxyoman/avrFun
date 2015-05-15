#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"
#include "Array.h"

using namespace Arduino;

int main(int argc, char *argv[]){
  
  auto value = GPIO<9,8>::read();
  GPIO<13,12>::writeValue(value.getValue());

  //GPIO<13>::setOuput();
  //Timer0::turnOn();
  //Timer0::setCompareA(0x80);
  //Timer0::setCompareB(0x40);
  //GPIO<5,6>::setAllOutput();
  //Timer0::setup(OC::Set, OC::Clear, WGM::Phase, CS::Clk);

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
