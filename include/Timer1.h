#pragma once
#include "LL/Register.h"
#include "PowerManager.h"
#include "Pin.h"
#include "Timer.h"

namespace Arduino{

//See page 133
namespace WGM1{
  constexpr auto Normal      = LL::BitSet<4>(0);
  constexpr auto Phase8      = LL::BitSet<4>(1);
  constexpr auto Phase9      = LL::BitSet<4>(2);
  constexpr auto Phase10     = LL::BitSet<4>(3);
  constexpr auto CTCA        = LL::BitSet<4>(4);
  constexpr auto Fast8       = LL::BitSet<4>(5);
  constexpr auto Fast9       = LL::BitSet<4>(6);
  constexpr auto Fast10      = LL::BitSet<4>(7);
  constexpr auto PhaseFreqIC = LL::BitSet<4>(8);
  constexpr auto PhaseFreqA  = LL::BitSet<4>(9);
  constexpr auto PhaseIC     = LL::BitSet<4>(10);
  constexpr auto PhaseA      = LL::BitSet<4>(11);
  constexpr auto CTCIC       = LL::BitSet<4>(12);
  constexpr auto FastIC      = LL::BitSet<4>(14);
  constexpr auto FastA       = LL::BitSet<4>(15);
}

class Timer1 {
  //Need to set the addresses correctly
  enum{
    tccra = 0x80,
    tccrb,
    tccrc,
    tcnt  = 0x84,
    icr   = 0x86,
    ocra  = 0x88,
    ocrb  = 0x8a,
    tifr  = 0x36,
    timsk = 0x6f,
  };

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
  using TCCRA = LL::Register<LL::Access::wr, tccra>;
  using TCCRB = LL::Register<LL::Access::wr, tccrb>;
  using TCCRC = LL::Register<LL::Access::wr, tccrc>;
  using FOCB  = TCCRC::template Bit<6>;
  using FOCA  = TCCRC::template Bit<7>;

  using TCNT = LL::Register<LL::Access::wr, tcnt, 0, 16, uint16_t>;
  using OCRA = LL::Register<LL::Access::wr, ocra, 0, 16, uint16_t>;
  using OCRB = LL::Register<LL::Access::wr, ocrb, 0, 16, uint16_t>;
  using ICR  = LL::Register<LL::Access::wr, icr, 0, 16, uint16_t>;

  using TIMSK = LL::Register<LL::Access::wr, timsk>;
  using TOIE  = TIMSK::template Bit<0>;
  using OCIEA = TIMSK::template Bit<1>;
  using OCIEB = TIMSK::template Bit<2>;
  using ICIE  = TIMSK::template Bit<5>;

  using TIFR  = LL::Register<LL::Access::wr, tifr>;
  using TOV   = TIFR::template Bit<0>;
  using OCFA  = TIFR::template Bit<1>;
  using OCFB  = TIFR::template Bit<2>;
  using ICF   = TIFR::template Bit<5>;

public:
  using OcADdr = digitalPin<6>::DDRBit;
  using OcBDdr = digitalPin<5>::DDRBit;

  AlwayInline static void turnOn(){
    PowerManager::turnOnTimer1();
  }
  AlwayInline static void turnOff(){
    PowerManager::turnOffTimer1();
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
  AlwayInline static bool didMatchIC(){
    return ICF::testAndSet();
  }

  AlwayInline static uint8_t getCount(){
    return TCNT::read();
  }

  //Set up the timer
  //See Timer.h for options, exept wgm look at the top of this file
  AlwayInline static void setup(const LL::BitSet<2> oca, 
      const LL::BitSet<2> ocb = OC::Normal, 
      const LL::BitSet<4> wgm = WGM::Normal, 
      const LL::BitSet<3> cs = CS::Clk){

    using tccra = TCCRA::template Bit<coma1, coma0, comb1, comb0, wgm1, wgm0>;
    using tccrb = TCCRB::template Bit<wgm3,  wgm2,  cs2,   cs1,   cs0>;
    tccra::wwrite(oca +  ocb + LL::BitSet<2>(wgm0));
    tccrb::wwrite(LL::BitSet<2>(wgm,2) + cs); 
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
