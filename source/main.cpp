#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"
#include "Array.h"

using namespace Arduino;

int main(int argc, char *argv[]){
  
  //PWM 
  GPIO<13>::setOuput();
  Timer0::turnOn();
  Timer0::setCompareA(0xa0);
  Timer0::setCompareB(0x40);
  GPIO<5,6>::setAllOutput();
  Timer0::setup(OC::Set, OC::Clear, WGM::Phase, CS::Clk);

  //bool value = 0;
  //while(1){
    //if(Timer0::didMatchA()){
      //GPIO<13>::write(value ^= 1);
    //}
  //}

  GPIO<13,12>::setAllOutput();
  Analog::init(REFS::Vref, ADPrescale::P2, true);
  Analog::setToADC<0>();
  while(1){
    auto value = Analog::read8<0>();
    Timer0::setCompareA(value);
    if(value > 200){
      GPIO<13>::write(Low);
    }else{
      GPIO<13>::write(High);
    }
  }

  while(1);
  return 0;
}
