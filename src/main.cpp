#include "LL/Register.h"
#include "LL/LoopSet.h"
#include "LL/Complex.h"
#include "Timer0.h"
#include "Analog.h"
#include "USART.h"
#include "avr/addresses.h"
#include "GPIO.h"
#include <avr/interrupt.h>

using Fixed16 = LL::Fixed<int16_t, 8>;
Fixed16 cos[32] = {
  1.0, 
  0.70710678118654757, 
  0, 
  -0.70710678118654746, 
  -1.0, 
  -0.70710678118654768, 
  0,
  0.70710678118654735,
  1.0, 
  0.70710678118654757, 
  0, 
  -0.70710678118654746, 
  -1.0, 
  -0.70710678118654768, 
  0,
  0.70710678118654735,
  1.0, 
  0.70710678118654757, 
  0, 
  -0.70710678118654746, 
  -1.0, 
  -0.70710678118654768, 
  0,
  0.70710678118654735,
  1.0, 
  0.70710678118654757, 
  0, 
  -0.70710678118654746, 
  -1.0, 
  -0.70710678118654768, 
  0,
  0.70710678118654735
};

Fixed16 sin[32] = {
  0, 
  -0.70710678118654746, 
  -1.0, 
  -0.70710678118654768, 
  0,
  0.70710678118654735,
  1.0, 
  0.70710678118654735,
  0, 
  -0.70710678118654746, 
  -1.0, 
  -0.70710678118654768, 
  0,
  0.70710678118654735,
  1.0, 
  0.70710678118654735,
  0, 
  -0.70710678118654746, 
  -1.0, 
  -0.70710678118654768, 
  0,
  0.70710678118654735,
  1.0, 
  0.70710678118654735,
  0, 
  -0.70710678118654746, 
  -1.0, 
  -0.70710678118654768, 
  0,
  0.70710678118654735,
  1.0, 
  0.70710678118654735
};

LL::Complex<Fixed16> out[4];

int main(){
  using namespace Arduino;

  USART::setup(9600);

  //Set up the ADC
  Analog::init(true, true);
  Analog::setToADC<0>();
  Analog::start();

  //set up the timer
  Timer0::turnOn();
  Timer0::setup(OC::Clear, OC::Normal, WGM::CTC, CS::Clk256);
  Timer0::turnOnIntrA();
  Timer0::setCompareA(0xff);
  GPIO<2,3,4,5>::setAllOutput();
  GPIO<8,9,10,11,12,13>::setAllOutput();

  sei();

  Fixed16 data[8];

  //Offset For around 1V
  Fixed16 offset;
  offset.value = 51;

  while(1){
    cli();
    //Get data
    for (int i = 0; i < 8; ++i){
      auto in = Analog::read8<0, int16_t>() - offset;
      data[i] = in;
    }
    sei();

    //Do the DFT
    for(int k = 0; k < 4; ++k){
      out[k] = out[k] * 0;
      for (int n = 0; n < 8; ++n){
        out[k] += LL::Complex<Fixed16>(cos[k*n], sin[k*n]) * data[n];
      }
    }

  }

  while(1);
}


ISR(TIMER0_COMPA_vect) {
  using namespace Arduino;
  static uint8_t currentCol = 0;
  LL::BitSet<4> output;
  auto dftValue = out[currentCol].abs2().value;
  if(dftValue > 10){
    output[0] = 1;
  }
  if(dftValue > 20){
    output[1] = 1;
  }
  if(dftValue > 40){
    output[2] = 1;
  }
  if(dftValue > 100){
    output[3] = 1;
  }
  GPIO<8,9,10,11>::write(output);

  if(currentCol == 0){
    GPIO<2, 3, 4, 5>::write(0,1,1,1);
    currentCol ++;
  }else if(currentCol == 1){
    GPIO<2, 3, 4, 5>::write(1,0,1,1);
    currentCol ++;
  }else if(currentCol == 2){
    GPIO<2, 3, 4, 5>::write(1,1,0,1);
    currentCol ++;
  }else if(currentCol == 3){
    GPIO<2, 3, 4, 5>::write(1,1,1,0);
    currentCol = 0;
  }
  Timer0::didMatchA();
}
