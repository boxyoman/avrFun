#include "USART.h"

using namespace Arduino;

int main(int argc, char *argv[]){

  USART::setup(9600);
  for (unsigned char i = 0; i < 0xff; ++i){
    USART::put(i);
  }

  while(1);
  return 0;
}
