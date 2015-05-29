#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"
#include "USART.h"

using namespace Arduino;

int main(int argc, char *argv[]){
  
  USART::setup(9600);

  Analog::init(true);
  Analog::setToADC<0>();

  USART::writeNum(Analog::read8<0>());
  USART::write("\r\n");

  while(1);
  return 0;
}
