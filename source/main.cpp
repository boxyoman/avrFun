#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"
#include "Array.h"

using namespace Arduino;

int main(int argc, char *argv[]){
  
  //PWM 
  Timer0::turnOn();
  Timer0::setCompareA(0x80);
  GPIO<5,6>::setAllOutput();
  Timer0::setup(OC::Set, OC::Normal, WGM::Phase, CS::Clk1024);

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
  Analog::init(ADRefs::Vref, ADPrescale::P2, true);
  Analog::setToADC<0>();
  while(1){
    auto value = Analog::read8<0>();
    while(!Timer0::didMatchA());
    Timer0::setCompareA(value);
    if(value >= 0x80){
      GPIO<13>::write(1);
    }else{
      GPIO<13>::write(0);
    }
  }

  while(1);
  return 0;
}
