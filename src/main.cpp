#include "GPIO.h"
#include "Timer0.h"
#include "Analog.h"
#include "USART.h"

using namespace Arduino;

int main(int argc, char *argv[]){
  
  USART::setup(9600);
  USART::put('a');
  
  USART::write("Hello World");

  while(1);
  return 0;
}
