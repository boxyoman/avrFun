#include "LL/Register.h"
#include "avr/addresses.h"

int main(int argc, char *argv[]){
  using portb = LL::Reg<avr::addr::portb>;
  portb::write<5>(1);
  while(1);
}
