#pragma once
#include "LL/Register.h"
#include "LL/RegSet.h"
#include "PowerManager.h"
#include "avr/addresses.h"
#include "Pin.h"
#include "Timer.h"

namespace Arduino{

//See page 133
namespace WGM1{
  struct T : LL::BitSet<4>{
    using LL::BitSet<4>::BitSet;
  };
  constexpr auto Normal      = T(0);
  constexpr auto Phase8      = T(1);
  constexpr auto Phase9      = T(2);
  constexpr auto Phase10     = T(3);
  constexpr auto CTCA        = T(4);
  constexpr auto Fast8       = T(5);
  constexpr auto Fast9       = T(6);
  constexpr auto Fast10      = T(7);
  constexpr auto PhaseFreqIC = T(8);
  constexpr auto PhaseFreqA  = T(9);
  constexpr auto PhaseIC     = T(10);
  constexpr auto PhaseA      = T(11);
  constexpr auto CTCIC       = T(12);
  constexpr auto FastIC      = T(14);
  constexpr auto FastA       = T(15);
}

class Timer1 {
  using a = avr::addr;
  using b = avr::bits::timer1;
  //Need to set the addresses correctly
  enum{
    wgm0  = 0,
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
    wgm3
  }; //Bit positions for the TCCR0A register

  //Probably won't even use half of these...
  //But who knows!
  using TCCRA = LL::Reg<a::tccra>;
  using TCCRB = LL::Reg<a::tccrb>;
  using TCCRC = LL::Reg<a::tccrc>;

  using TCNT = LL::Reg<a::tcnt, uint16_t>;
  using OCRA = LL::Reg<a::ocra, uint16_t>;
  using OCRB = LL::Reg<a::ocrb, uint16_t>;
  using ICR  = LL::Reg<a::icr,  uint16_t>;
  using TIMSK = LL::Reg<a::timsk>;
  using TIFR  = LL::Reg<a::tifr>;

  using OcADdr = digitalPin<6>::DDR;
  using OcBDdr = digitalPin<5>::DDR;
public:

  AlwayInline static void turnOn(){
    PowerManager::turnOnTimer1();
  }
  AlwayInline static void turnOff(){
    PowerManager::turnOffTimer1();
  }

  AlwayInline static void forceA(){
    TCCRA::write<b::foca>(1);
  }
  AlwayInline static void forceB(){
    TCCRA::write<b::foca>(1);
  }

  AlwayInline static void turnOffIntr(){
    TIMSK::write(0);
  }

  AlwayInline static void setCount(uint8_t value){
    TCNT::write(value);
  }

  AlwayInline static bool countedOver(){
    return TCCRA::testAndSet<b::tov>();
  }

  AlwayInline static bool didMatchA(){
    return TCCRA::testAndSet<b::ocfa>();
  }
  AlwayInline static bool didMatchB(){
    return TCCRA::testAndSet<b::ocfb>();
  }
  AlwayInline static bool didMatchIC(){
    return TCCRA::testAndSet<b::icf>();
  }

  AlwayInline static uint8_t getCount(){
    return TCNT::read().getValue();
  }

  //Set up the timer
  //See Timer.h for options, exept wgm look at the top of this file
  AlwayInline static void setup(const LL::BitSet<2> oca, 
      const LL::BitSet<2> ocb = OC::Normal, 
      const WGM1::T wgm = WGM::Normal, 
      const LL::BitSet<3> cs = CS::Clk){

    auto ta = LL::RegSet<a::tccr0a, false>();
    auto tb = LL::RegSet<a::tccr0b, false>();
    ta.write<b::coma1, b::coma0>(oca);
    ta.write<b::comb1, b::comb0>(ocb);

    ta.write<b::wgm1, b::wgm0>(wgm);
    tb.write<b::wgm3, b::wgm2>(wgm>>2);

    tb.write<b::cs2, b::cs1, b::cs0>(cs);
  }

  AlwayInline static void setCompareA(uint16_t value){
    OCRA::write(value);
  }
  AlwayInline static void setCompareB(uint16_t value){
    OCRB::write(value);
  }
  AlwayInline static void setIC(uint16_t value){
    ICR::write(value);
  }

};
}//end of Arduino
