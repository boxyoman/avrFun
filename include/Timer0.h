#pragma once
#include "LL/Register.h"
#include "LL/RegSet.h"
#include "PowerManager.h"
#include "Pin.h"
#include "Timer.h"

namespace Arduino{

class Timer0 {
  using addr = avr::addr;
  using bit = avr::bits::timer0;

  //Probably won't even use half of these...
  //But who knows!
  using TCCRA = LL::Register<LL::Access::wr, addr::tccr0a>;
  using TCCRB = LL::Register<LL::Access::wr, addr::tccr0b>;
  using FOCB  = TCCRB::template Bit<6>;
  using FOCA  = TCCRB::template Bit<7>;

  using TCNT  = LL::Register<LL::Access::wr, addr::tcnt0>;
  using OCRA  = LL::Register<LL::Access::wr, addr::ocr0a>;
  using OCRB  = LL::Register<LL::Access::wr, addr::ocr0b>;

  using TIMSK = LL::Register<LL::Access::wr, addr::timsk0>;
  using TOIE  = TIMSK::template Bit<0>;
  using OCIEA = TIMSK::template Bit<1>;
  using OCIEB = TIMSK::template Bit<2>;

  using TIFR  = LL::Register<LL::Access::wr, addr::tifr0>;
  using TOV   = TIFR::template Bit<0>;
  using OCFA  = TIFR::template Bit<1>;
  using OCFB  = TIFR::template Bit<2>;


public:
  using OcADdr = digitalPin<6>::DDRBit;
  using OcBDdr = digitalPin<5>::DDRBit;

  AlwayInline static void turnOn(){
    PowerManager::turnOnTimer0();
  }
  AlwayInline static void turnOff(){
    PowerManager::turnOffTimer0();
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
    return OCIEB::testAndSet();
  }

  AlwayInline static uint8_t getCount(){
    return TCNT::read();
  }

  //Set up the timer
  //See Timer.h
  AlwayInline static void setup(const LL::BitSet<2> oca, 
      const LL::BitSet<2> ocb = OC::Normal, 
      const LL::BitSet<3> wgm = WGM::Normal, 
      const LL::BitSet<3> cs = CS::Clk){

    auto ta = LL::RegSet<addr::tccr0a, false>();
    auto tb = LL::RegSet<addr::tccr0b, false>();
    ta.write<bit::coma1, bit::coma0>(oca);
    ta.write<bit::comb1, bit::comb0>(ocb);

    ta.write<bit::wgm1, bit::wgm0>(wgm);
    tb.write<bit::wgm2>(wgm>>2);

    tb.write<bit::cs2, bit::cs1, bit::cs0>(cs);
  }

  AlwayInline static void setCompareA(uint8_t value){
    OCRA::wwrite(value);
  }
  AlwayInline static void setCompareB(uint8_t value){
    OCRB::wwrite(value);
  }

};
}//end of Arduino
