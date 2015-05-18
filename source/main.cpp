#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"
#include "Array.h"

using namespace Arduino;

int main(int argc, char *argv[]){
  
  //PWM 
  GPIO<13>::setOuput();
  Timer0::turnOn();
  Timer0::setCompareA(0x80);
  GPIO<5,6>::setAllOutput();
  Timer0::setup(OC::Set, OC::Normal, WGM::Fast, CS::Clk);

  //uint8_t test = 0;
  //while(1){
    //if(Timer0::didMatchA()){
      //Timer0::setCompareA(test++);
      //if(test == 0xff){
        //test = 0;
      //}
    //}
  //}

  GPIO<13,12>::setAllOutput();
  Analog::init();
  Analog::setToADC<0>();
  while(1){
    auto value = Analog::read8<0>();
    Timer0::setCompareA(value);
  }

  while(1);
  return 0;
}
