#pragma once
#include "Register.h"
#include "PowerManager.h"
#include "Pin.h"
#include "templateList.h"
#include "Timer.h"

namespace Arduino{

class Timer2 {
  enum{
    tccr2a = 0xb0,
    tccr2b,
    tcnt2,
    ocr2a,
    ocr2b,
    tifr2  = 0x37,
    timsk2 = 0x70,
  };

  enum{
    wgm20 = 0,
    wgm21,
    com2b0 = 4,
    com2b1,
    com2a0,
    com2a1
  }; //Bit positions for the TCCR0A register
  enum{
    cs20 = 0,
    cs21,
    cs22,
    wgm22,
  };
  //COM0A0 Define Registers
  //COM0A1 Probably won't even use half of them...
  //But who knows!
  using TCCR2A = LL::Register<LL::Access::wr, tccr2a>;
  using TCCR2B = LL::Register<LL::Access::wr, tccr2b>;
  using FOC2B  = TCCR2B::template Bit<6>;
  using FOC2A  = TCCR2B::template Bit<7>;

  using TCNT2  = LL::Register<LL::Access::wr, tcnt2>;
  using OCR2A  = LL::Register<LL::Access::wr, ocr2a>;
  using OCR2B  = LL::Register<LL::Access::wr, ocr2b>;

  using TIMSK2 = LL::Register<LL::Access::wr, timsk2>;
  using TOIE2  = TIMSK2::template Bit<0>;
  using OCIE2A = TIMSK2::template Bit<1>;
  using OCIE2B = TIMSK2::template Bit<2>;

  using TIFR2  = LL::Register<LL::Access::wr, tifr2>;
  using TOV2   = TIFR2::template Bit<0>;
  using OCF2A  = TIFR2::template Bit<1>;
  using OCF2B  = TIFR2::template Bit<2>;


public:
  using Oc2aDdr = digitalPin<11>::DDRBit;
  using Oc2bDdr = digitalPin<3>::DDRBit;

  AlwayInline static void turnOn(){
    PowerManager::turnOnTimer2();
  }
  AlwayInline static void turnOff(){
    PowerManager::turnOffTimer2();
  }

  AlwayInline static void forceA(){
    FOC2A::write(1);
  }
  AlwayInline static void forceB(){
    FOC2B::write(1);
  }

  AlwayInline static void turnOffIntr(){
    TIMSK2::write(0);
  }

  AlwayInline static void setCount(uint8_t value){
    TCNT2::write(value);
  }

  AlwayInline static bool countedOver(){
    return TOV2::read()[0];
  }

  //These don't work yet...
  AlwayInline static bool didMatchA(){
    auto value = OCIE2A::read()[0];
    if(value) OCIE2A::wwrite(value);
    return value == 1;
  }
  AlwayInline static bool didMatchB(){
    auto value = OCIE2B::read()[0];
    if(value) OCIE2B::wwrite(value);
    return value == 1;
  }

  AlwayInline static uint8_t getCount(){
    return TCNT2::read();
  }

  //Set up the timer
  //See Timer.h for options
  AlwayInline static void setup(const LL::BitSet<2> oca, 
      const LL::BitSet<2> ocb = OC::Normal, 
      const LL::BitSet<3> wgm = WGM::Normal, 
      const LL::BitSet<3> cs = CS::Clk){

    using tccra = TCCR2A::template Bit<com2a1, com2a0, com2b1, com2b0, 
          wgm21, wgm20>;
    using tccrb = TCCR2B::template Bit<wgm22, cs22, cs21, cs20>;

    tccra::wwrite(oca +  ocb + LL::BitSet<2>(wgm));
    tccrb::wwrite(LL::BitSet<1>(wgm,2) + cs); 
  }

  AlwayInline static void setCompareA(uint8_t value){
    OCR2A::write(value);
  }
  AlwayInline static void setCompareB(uint8_t value){
    OCR2B::write(value);
  }
};
}//end of Arduino
