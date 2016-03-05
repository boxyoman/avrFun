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

  auto value2 = LL::Fixed<uint16_t, 8>(2.0f);
  auto value4 = LL::Fixed<uint16_t, 8>(2.0f);

  while(1){
    //auto test = Analog::read8<0, uint16_t>();
    auto test = value4.mult(value2).value;
    USART::writeNum(test);
    USART::write("\r\n");
  }

  while(1);
}
