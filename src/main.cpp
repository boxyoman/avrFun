#include "USART.h"
#include "GPIO.h"

using namespace Arduino;

int main(int argc, char *argv[]){

  auto a = GPIO<9,8>::read();
  GPIO<12,11>::write(a);

  //USART::setup(9600);
  //for (unsigned char i = 0; i < 0xff; ++i){
    //USART::put(i);
  //}

  while(1);
  return 0;
}
