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
  constexpr auto Normal = LowLevel::BitSet(0,0,0);
  constexpr auto Toggle = LowLevel::BitSet(0,0,1);
  constexpr auto Set    = LowLevel::BitSet(0,1,0);
  constexpr auto Clear  = LowLevel::BitSet(0,1,1);
}
//Wave Form Generation Modes
//page 107
namespace WGM{
  constexpr auto Normal = LowLevel::BitSet(0,0,0);
  constexpr auto CTC = LowLevel::BitSet(0,1,0); //Clear time on compare
  constexpr auto Fast = LowLevel::BitSet(0,1,1);
  constexpr auto FastA = LowLevel::BitSet(1,0,1);
};
//Clock Select
namespace CS{
  constexpr auto NoClk   = LowLevel::BitSet(0,0,0);
  constexpr auto Clk     = LowLevel::BitSet(0,0,1);
  constexpr auto Clk8    = LowLevel::BitSet(0,1,0);
  constexpr auto Clk64   = LowLevel::BitSet(0,1,1);
  constexpr auto Clk256  = LowLevel::BitSet(1,0,0);
  constexpr auto Clk1024 = LowLevel::BitSet(1,0,1);
  constexpr auto ExtRise = LowLevel::BitSet(1,1,0);
  constexpr auto ExtFall = LowLevel::BitSet(1,1,1);
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
  using TCCR0A = LowLevel::Register<LowLevel::Access::wr, tccr0a>;
  using TCCR0B = LowLevel::Register<LowLevel::Access::wr, tccr0b>;
  using FOC0B  = TCCR0B::template Bit<6>;
  using FOC0A  = TCCR0B::template Bit<7>;

  using TCNT0  = LowLevel::Register<LowLevel::Access::wr, tcnt0>;
  using OCR0A  = LowLevel::Register<LowLevel::Access::wr, ocr0a>;
  using OCR0B  = LowLevel::Register<LowLevel::Access::wr, ocr0b>;

  using TIMSK0 = LowLevel::Register<LowLevel::Access::wr, timsk0>;
  using TOIE0  = TIMSK0::template Bit<0>;
  using OCIE0A = TIMSK0::template Bit<1>;
  using OCIE0B = TIMSK0::template Bit<2>;

  using TIFR0  = LowLevel::Register<LowLevel::Access::wr, tifr0>;
  using TOV0   = TIFR0::template Bit<0>;
  using OCF0A  = TIFR0::template Bit<1>;
  using OCF0B  = TIFR0::template Bit<2>;


public:
  using Oc0aDdr = digitalPin<6>::DDRBit;
  using Oc0bDdr = digitalPin<5>::DDRBit;

  static void turnOn(){
    PowerManager::turnOnTimer0();
  }
  static void forceA(){
    FOC0A::write(1);
  }

  static void turnOffIntr(){
    TIMSK0::write(0);
  }
  static void setCount(uint8_t value){
    TCNT0::write(value);
  }
  static bool countedOver(){
    bool value;
    TOV0::read(value);
    return value;
  }

  static bool didMatchA(){
    uint8_t value;
    OCIE0A::read(value);
    if(value) OCIE0A::write(value);
    return value == 1;
  }
  static uint8_t getCount(){
    return TCNT0::read();
  }

  //Set up the data
  static void setup(const LowLevel::BitSet oc0a, 
      const LowLevel::BitSet oc0b = OC::Normal, 
      const LowLevel::BitSet wgm0 = WGM::Normal, 
      const LowLevel::BitSet cs = CS::Clk){

    using tccra = TCCR0A::template Bit<com0a1, com0a0, com0b1, com0b0, 
          wgm01, wgm00>;
    using tccrb = TCCR0B::template Bit<wgm02, cs02, cs01, cs00>;

    //Looks ugly but it gets the job done.
    tccra::wwrite(oc0a[1], oc0a[0], oc0b[1], oc0b[0], wgm0[1], wgm0[0]);
    
    tccrb::wwrite(wgm0[2], cs[2], cs[1], cs[0]); 
  }

  static void setCompareA(uint8_t value){
    OCR0A::write(value);
  }
  static void setCompareB(uint8_t value){
    OCR0B::write(value);
  }
};
}//end of Arduino
