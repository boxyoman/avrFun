#include "LL/Register.h"
#include "avr/addresses.h"

int main(int argc, char *argv[]){
  using portb = LL::reg<avr::addr::portb>;
  portb::write(1<<5);
  while(1);
}
