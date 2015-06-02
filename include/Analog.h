#pragma once

#include "LL/Register.h"
#include "PowerManager.h"
#include "Pin.h"
#include "LL/RegSet.h"
#include "avr/addresses.h"

namespace Arduino{

namespace ADRefs{
  //Allows for easy type checking
  struct T : LL::BitSet<2>{
    using LL::BitSet<2>::BitSet;
  };
  constexpr auto Aref     = T(0);
  constexpr auto AVcc     = T(1);
  constexpr auto Internal = T(3);
}

namespace ADPrescale{
  struct T : LL::BitSet<3>{
    using LL::BitSet<3>::BitSet;
  };
  constexpr auto P2   = T(1);
  constexpr auto P4   = T(2);
  constexpr auto P8   = T(3);
  constexpr auto P16  = T(4);
  constexpr auto P32  = T(5);
  constexpr auto P64  = T(6);
  constexpr auto P128 = T(7);
}

class Analog {
  using addr = avr::addr;
  using bit = avr::bits::adc;

  //useful registers
  using ADMUX = LL::Register<LL::Access::wr, addr::admux>;
  using MUX = typename ADMUX::template Bit<3,2,1,0>;

  using ADCSRAReg = LL::Register<LL::Access::wr, addr::adcsra>;
  using ADEN    = typename ADCSRAReg::template Bit<bit::aden>;
  using ADSCReg = typename ADCSRAReg::template Bit<bit::adsc>;
  using ADATE   = typename ADCSRAReg::template Bit<bit::adate>;
  using ADIFReg = typename ADCSRAReg::template Bit<bit::adif>;
  using ADIE    = typename ADCSRAReg::template Bit<bit::adie>;
  using ADPS    = typename ADCSRAReg::template Bit<bit::adps2,bit::adps1,
        bit::adps0>;

  using Data = LL::Register<LL::Access::wr, addr::adcl, 0, 16, uint16_t>;
  using ADCL = LL::Register<LL::Access::wr, addr::adcl>;
  using ADCH = LL::Register<LL::Access::wr, addr::adch>;

  using ADCSRB = LL::Register<LL::Access::wr, addr::adcsrb>;
  using ACME = typename ADCSRB::template Bit<6>;
  using ADTS = typename ADCSRB::template Bit<2,1,0>;

  using DIDR0 = LL::Register<LL::Access::wr, addr::didr0>;

public:
  
  //initialize ADC
  AlwayInline static void init(const ADRefs::T ref = ADRefs::AVcc, 
      bool alignLeft = false,
      bool autoReload = false){

    //Define some variables
    auto ADCSRA = LL::RegSet<addr::adcsra>();
    auto mux = LL::RegSet<addr::admux>();

    //Turn on ADC
    PowerManager::turnOnAdc();

    //Set Ref voltage 
    mux.write<7,6>(ref);
    //set alignment
    mux.write<5>(alignLeft);

    //Enble ADC
    ADCSRA.write<bit::aden>(1);
    ADCSRA.write<bit::adate>(autoReload);
    //set prescale
    ADCSRA.write<bit::adps2,bit::adps1,bit::adps0>(ADPrescale::P128);

  }

  AlwayInline static void init(bool alignLeft,
      const bool autoReload = false,
      const ADRefs::T ref = ADRefs::AVcc 
      ){
    init(ref, alignLeft, autoReload);
  }

  //Set pin to an adc input
  template<unsigned pin>
  AlwayInline static void setToADC(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    auto didr = LL::RegSet<addr::didr0>();
    didr.write<actPin>(1);
  }

  AlwayInline static void start(){
    ADSCReg::write(1);
  }

  //Read the output 
  //use when left align is true
  template<unsigned pin>
  AlwayInline static uint8_t read8(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    MUX::write(LL::BitSet<4>(actPin));
    while(!ADIFReg::testAndSet()); //wait for conversion to be done
    
    return ADCH::read();
  }

  //Read the output
  //Use when left align is false
  template<unsigned pin>
  AlwayInline static uint16_t read(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    MUX::write(LL::BitSet<4>(actPin));
    while(!ADIFReg::testAndSet()); //wait for conversion to be done
    
    return Data::read();
  }

}; //End of Analog
} //End of Arduino
