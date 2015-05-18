#pragma once
#include "Register.h"
#include "PowerManager.h"
#include "Pin.h"
#include "templateList.h"
#include "Timer.h"

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

  enum{
    wgm00 = 0,
    wgm01,
    com0b0 = 4,
    com0b1,
    com0a0,
    com0a1
  }; //Bit positions for the TCCR0A register

  enum{
    cs00 = 0,
    cs01,
    cs02,
    wgm02,
  }; //Bit positions for the TCCR0A register
  //COM0A0 Define Registers
  //COM0A1 Probably won't even use half of them...
  //But who knows!
  using TCCR0A = LL::Register<LL::Access::wr, tccr0a>;
  using TCCR0B = LL::Register<LL::Access::wr, tccr0b>;
  using FOC0B  = TCCR0B::template Bit<6>;
  using FOC0A  = TCCR0B::template Bit<7>;

  using TCNT0  = LL::Register<LL::Access::wr, tcnt0>;
  using OCR0A  = LL::Register<LL::Access::wr, ocr0a>;
  using OCR0B  = LL::Register<LL::Access::wr, ocr0b>;

  using TIMSK0 = LL::Register<LL::Access::wr, timsk0>;
  using TOIE0  = TIMSK0::template Bit<0>;
  using OCIE0A = TIMSK0::template Bit<1>;
  using OCIE0B = TIMSK0::template Bit<2>;

  using TIFR0  = LL::Register<LL::Access::wr, tifr0>;
  using TOV0   = TIFR0::template Bit<0>;
  using OCF0A  = TIFR0::template Bit<1>;
  using OCF0B  = TIFR0::template Bit<2>;


public:
  using Oc0aDdr = digitalPin<6>::DDRBit;
  using Oc0bDdr = digitalPin<5>::DDRBit;

  AlwayInline static void turnOn(){
    PowerManager::turnOnTimer0();
  }
  AlwayInline static void turnOff(){
    PowerManager::turnOffTimer0();
  }

  AlwayInline static void forceA(){
    FOC0A::write(1);
  }
  AlwayInline static void forceB(){
    FOC0B::write(1);
  }

  AlwayInline static void turnOffIntr(){
    TIMSK0::write(0);
  }

  AlwayInline static void setCount(uint8_t value){
    TCNT0::write(value);
  }

  AlwayInline static bool countedOver(){
    return TOV0::testAndSet();
  }

  AlwayInline static bool didMatchA(){
    return OCF0A::testAndSet();
  }

  AlwayInline static bool didMatchB(){
    return OCIE0B::testAndSet();
  }

  AlwayInline static uint8_t getCount(){
    return TCNT0::read();
  }

  //Set up the timer
  //See Timer.h
  AlwayInline static void setup(const LL::BitSet<2> oc0a, 
      const LL::BitSet<2> oc0b = OC::Normal, 
      const LL::BitSet<3> wgm0 = WGM::Normal, 
      const LL::BitSet<3> cs = CS::Clk){

    using tccra = TCCR0A::template Bit<com0a1, com0a0, com0b1, com0b0, 
          wgm01, wgm00>;
    using tccrb = TCCR0B::template Bit<wgm02, cs02, cs01, cs00>;

    tccra::wwrite(oc0a +  oc0b + LL::BitSet<2>(wgm0));
    tccrb::wwrite(LL::BitSet<1>(wgm0,2) + cs); 
  }

  AlwayInline static void setCompareA(uint8_t value){
    OCR0A::write(value);
  }
  AlwayInline static void setCompareB(uint8_t value){
    OCR0B::write(value);
  }
};
}//end of Arduino
