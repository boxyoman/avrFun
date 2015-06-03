#pragma once
#include "LL/Register.h"
#include "LL/RegSet.h"
#include "PowerManager.h"
#include "Pin.h"
#include "Timer.h"

namespace Arduino{

class Timer2 {
  using a = avr::addr;
  using b = avr::bits::timer2;

  //Probably won't even use half of these...
  //But who knows!
  using TCCRA = LL::Reg<a::tccr2a>;
  using TCCRB = LL::Reg<a::tccr2b>;
  using TCNT  = LL::Reg<a::tcnt2>;
  using OCRA  = LL::Reg<a::ocr2a>;
  using OCRB  = LL::Reg<a::ocr2b>;
  using TIMSK = LL::Reg<a::timsk2>;
  using TIFR  = LL::Reg<a::tifr2>;

  using OcADdr = digitalPin<11>::DDR;
  using OcBDdr = digitalPin<3>::DDR;

public:

  AlwayInline static void turnOn(){
    PowerManager::turnOnTimer0();
  }
  AlwayInline static void turnOff(){
    PowerManager::turnOffTimer0();
  }

  AlwayInline static void forceA(){
    TCCRA::write<b::foca>(1);
  }
  AlwayInline static void forceB(){
    TCCRA::write<b::focb>(1);
  }

  AlwayInline static void turnOffIntr(){
    TIMSK::write(0);
  }

  AlwayInline static void setCount(uint8_t value){
    TCNT::write(value);
  }

  AlwayInline static bool countedOver(){
    return TIFR::testAndSet<b::tov>();
  }

  AlwayInline static bool didMatchA(){
    return TIFR::testAndSet<b::ocfa>();
  }

  AlwayInline static bool didMatchB(){
    return TIFR::testAndSet<b::ocfb>();
  }

  AlwayInline static uint8_t getCount(){
    return TCNT::read().getValue();
  }

  //Set up the timer
  //See Timer.h
  AlwayInline static void setup(const LL::BitSet<2> oca, 
      const LL::BitSet<2> ocb = OC::Normal, 
      const LL::BitSet<3> wgm = WGM::Normal, 
      const LL::BitSet<3> cs = CS::Clk){

    auto ta = LL::RegSet<a::tccr0a, false>();
    auto tb = LL::RegSet<a::tccr0b, false>();
    ta.write<b::coma1, b::coma0>(oca);
    ta.write<b::comb1, b::comb0>(ocb);

    ta.write<b::wgm1, b::wgm0>(wgm);
    tb.write<b::wgm2>(wgm>>2);

    tb.write<b::cs2, b::cs1, b::cs0>(cs);
  }

  AlwayInline static void setCompareA(uint8_t value){
    OCRA::wwrite(value);
  }
  AlwayInline static void setCompareB(uint8_t value){
    OCRB::wwrite(value);
  }

};
}
