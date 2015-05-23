#pragma once
#include "LL/Register.h"
#include "PowerManager.h"
#include "Pin.h"
#include "Timer.h"

namespace Arduino{

class Timer2 {
  enum{
    tccra = 0xb0,
    tccrb,
    tcnt,
    ocra,
    ocrb,
    tifr  = 0x37,
    timsk = 0x70,
  };

  enum{
    wgm0 = 0,
    wgm1,
    comb0 = 4,
    comb1,
    coma0,
    coma1
  }; //Bit positions for the TCCR0A register
  enum{
    cs0 = 0,
    cs1,
    cs2,
    wgm2,
  };

  //Probably won't even use half of these...
  //But who knows!
  using TCCRA = LL::Register<LL::Access::wr, tccra>;
  using TCCRB = LL::Register<LL::Access::wr, tccrb>;
  using FOCB  = TCCRB::template Bit<6>;
  using FOCA  = TCCRB::template Bit<7>;

  using TCNT  = LL::Register<LL::Access::wr, tcnt>;
  using OCRA  = LL::Register<LL::Access::wr, ocra>;
  using OCRB  = LL::Register<LL::Access::wr, ocrb>;

  using TIMSK = LL::Register<LL::Access::wr, timsk>;
  using TOIE  = TIMSK::template Bit<0>;
  using OCIEA = TIMSK::template Bit<1>;
  using OCIEB = TIMSK::template Bit<2>;

  using TIFR  = LL::Register<LL::Access::wr, tifr>;
  using TOV   = TIFR::template Bit<0>;
  using OCFA  = TIFR::template Bit<1>;
  using OCFB  = TIFR::template Bit<2>;

public:
  using OcADdr = digitalPin<11>::DDRBit;
  using OcBDdr = digitalPin<3>::DDRBit;

  AlwayInline static void turnOn(){
    PowerManager::turnOnTimer2();
  }
  AlwayInline static void turnOff(){
    PowerManager::turnOffTimer2();
  }

  AlwayInline static void forceA(){
    FOCA::write(1);
  }
  AlwayInline static void forceB(){
    FOCB::write(1);
  }

  AlwayInline static void turnOffIntr(){
    TIMSK::write(0);
  }

  AlwayInline static void setCount(uint8_t value){
    TCNT::write(value);
  }

  AlwayInline static bool countedOver(){
    return TOV::testAndSet();
  }

  AlwayInline static bool didMatchA(){
    return OCFA::testAndSet();
  }
  AlwayInline static bool didMatchB(){
    return OCFB::testAndSet();
  }

  AlwayInline static uint8_t getCount(){
    return TCNT::read();
  }

  //Set up the timer
  //See Timer.h for options
  AlwayInline static void setup(const LL::BitSet<2> oca, 
      const LL::BitSet<2> ocb = OC::Normal, 
      const LL::BitSet<3> wgm = WGM::Normal, 
      const LL::BitSet<3> cs = CS::Clk){

    using tccra = TCCRA::template Bit<coma1, coma0, comb1, comb0, wgm1, wgm0>;
    using tccrb = TCCRB::template Bit<wgm2, cs2, cs1, cs0>;

    tccra::wwrite(oca +  ocb + LL::BitSet<2>(wgm));
    tccrb::wwrite(LL::BitSet<1>(wgm,2) + cs); 
  }

  AlwayInline static void setCompareA(uint8_t value){
    OCRA::write(value);
  }
  AlwayInline static void setCompareB(uint8_t value){
    OCRB::write(value);
  }

};
}//end of Arduino
