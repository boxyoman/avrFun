#include "LL/Register.h"
#include "avr/addresses.h"
#include "GPIO.h"

int main(int argc, char *argv[]){
  using namespace Arduino;
  GPIO<13, 8>::writeValue(0b11);
  GPIO<13>::write(0);

  //using portb = LL::Reg<avr::addr::portb, 1, 5>;
  //auto a = portb::read();
  //portb::wwrite(a);
  while(1);
}
