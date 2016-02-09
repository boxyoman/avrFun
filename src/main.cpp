#include "LL/Register.h"
#include "Analog.h"
#include "USART.h"
#include "avr/addresses.h"
#include "GPIO.h"

int main(){
  using namespace Arduino;

  USART::setup(9600);

  Analog::init(true, true);
  Analog::setToADC<0>();
  Analog::start();

  while(1){
    USART::writeNum(Analog::read8<0>());
    USART::write("\r\n");
  }

  while(1);
}
