#include "GPIO.h"
#include "Analog.h"
#include "USART.h"

int main(){
  using namespace Arduino; 
  GPIO<13>::setOuput();
  GPIO<13>::write(0);

  USART::setup(9600);

  Analog::init(true, true);
  Analog::setToADC<0>();
  Analog::setToADC<1>();
  Analog::setToADC<2>();
  Analog::setToADC<3>();
  Analog::start();

  auto read = Analog::read8<0, uint16_t>();
  USART::writeNum(read.value);
  USART::write("\n\r");

  read = Analog::read8<1, uint16_t>();
  USART::writeNum(read.value);
  USART::write("\n\r");

  read = Analog::read8<2, uint16_t>();
  USART::writeNum(read.value);
  USART::write("\n\r");

  read = Analog::read8<3, uint16_t>();
  USART::writeNum(read.value);
  USART::write("\n\r");
  USART::write("\n\r");

  return 0;
}

