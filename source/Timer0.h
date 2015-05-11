#pragma once
#include "Register.h"
#include "PowerManager.h"
#include "Pin.h"

namespace Arduino{
class Timer0 {
  enum{
    tccr0a = 0x44,
    tccr0b = 0x45,
    tcnt0  = 0x46,
    ocr0a  = 0x47,
    ocr0b  = 0x48,
    tifr0  = 0x35,
    timsk0 = 0x6e,
  };


  //Define Registers
  //Probably won't even use half of them...
  //But who knows!
  using TCCR0A = LowLevel::Register<LowLevel::Access::wr, tccr0a>;
  using WGM00  = TCCR0A::template Bit<0>;
  using WGM01  = TCCR0A::template Bit<1>;
  using COM0B0 = TCCR0A::template Bit<4>;
  using COM0B1 = TCCR0A::template Bit<5>;
  using COM0A0 = TCCR0A::template Bit<6>;
  using COM0A1 = TCCR0A::template Bit<7>;
  using TCCR0B = LowLevel::Register<LowLevel::Access::wr, tccr0b>;
  using CS00   = TCCR0B::template Bit<0>;
  using CS01   = TCCR0B::template Bit<1>;
  using CS02   = TCCR0B::template Bit<2>;
  using WGM02  = TCCR0B::template Bit<3>;
  using FOC0B  = TCCR0B::template Bit<6>;
  using FOC0A  = TCCR0B::template Bit<7>;
  using TCNT0  = LowLevel::Register<LowLevel::Access::wr, tcnt0>;
  using OCR0A  = LowLevel::Register<LowLevel::Access::wr, ocr0a>;
  using OCR0B  = LowLevel::Register<LowLevel::Access::wr, ocr0b>;
  using TIFR0  = LowLevel::Register<LowLevel::Access::wr, tifr0>;
  using TOV0   = TIFR0::template Bit<0>;
  using OCF0A  = TIFR0::template Bit<1>;
  using OCF0B  = TIFR0::template Bit<2>;
  using TIMSK0 = LowLevel::Register<LowLevel::Access::wr, timsk0>;
  using TOIE0  = TIMSK0::template Bit<0>;
  using OCIE0A = TIMSK0::template Bit<0>;
  using OCIE0B = TIMSK0::template Bit<0>;

  using OC0ADDR = Pin<PinTypes::Digital, 6>::DDRBit;
  using OC0BDDR = Pin<PinTypes::Digital, 5>::DDRBit;



public:
  static void init(){
    PowerManager::turnOnTimer0();
  }
};
}//end of Arduino
