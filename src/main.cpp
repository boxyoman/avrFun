#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"
#include "USART.h"
#include "LL/RegSet.h"

#define test 3,2,1,0

using namespace Arduino;

int main(int argc, char *argv[]){
  
  USART::setup(9600);

  Analog::init();
  Analog::setToADC<0>();

  USART::writeNum(Analog::read<0>());
  USART::write("\r\n");

  while(1);
  return 0;
}
