#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"
#include "USART.h"
//#include "LL/RegSet.h"
//#include "avr/addresses.h"
//#include "LL/Access.h"


int main(int argc, char *argv[]){
  using namespace Arduino;

  Timer0::setCompareA(0x80);
  Timer0::setup(OC::Set, OC::Normal, WGM::Fast, CS::Clk8);
  Timer0::OcADdr::wwrite(Output);
  Timer0::turnOn();

  USART::setup(9600);

  Analog::init(false, true);
  Analog::setToADC<0>();
  Analog::start();

  while(1){
    USART::writeNum(Analog::read<0>());
    USART::write("\r\n");
  }

  while(1);
}
