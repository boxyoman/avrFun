#pragma once
#include "Register.h"
#include "PowerManager.h"
#include "Pin.h"
#include "templateList.h"
#include "BitSet.h"
#include <bitset>

namespace Arduino{
//OCxn modes of operation
//page 105
namespace OC{
  constexpr auto Normal = LL::BitSet<2>(0);
  constexpr auto Toggle = LL::BitSet<2>(1);
  constexpr auto Set    = LL::BitSet<2>(2);
  constexpr auto Clear  = LL::BitSet<2>(3);
}
//Wave Form Generation Modes
//page 107
namespace WGM{
  constexpr auto Normal   = LL::BitSet<3>(0);
  constexpr auto Phase    = LL::BitSet<3>(1);
  constexpr auto CTC      = LL::BitSet<3>(2); //Clear time on compare
  constexpr auto Fast     = LL::BitSet<3>(3);
  constexpr auto PhaseToA = LL::BitSet<3>(0b101);
  constexpr auto FastToA  = LL::BitSet<3>(0b111);
};
//Clock Select
namespace CS{
  constexpr auto NoClk   = LL::BitSet<3>(0);
  constexpr auto Clk     = LL::BitSet<3>(1);
  constexpr auto Clk8    = LL::BitSet<3>(2);
  constexpr auto Clk64   = LL::BitSet<3>(3);
  constexpr auto Clk256  = LL::BitSet<3>(4);
  constexpr auto Clk1024 = LL::BitSet<3>(5);
  constexpr auto ExtRise = LL::BitSet<3>(6);
  constexpr auto ExtFall = LL::BitSet<3>(7);
};

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
  };
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
  AlwayInline static void forceA(){
    FOC0A::write(1);
  }

  AlwayInline static void turnOffIntr(){
    TIMSK0::write(0);
  }
  AlwayInline static void setCount(uint8_t value){
    TCNT0::write(value);
  }
  AlwayInline static bool countedOver(){
    bool value;
    TOV0::read(value);
    return value;
  }

  AlwayInline static bool didMatchA(){
    uint8_t value;
    OCIE0A::read(value);
    if(value) OCIE0A::write(value);
    return value == 1;
  }
  AlwayInline static uint8_t getCount(){
    return TCNT0::read();
  }

  //Set up the data
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
