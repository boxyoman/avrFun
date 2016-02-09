#pragma once

#include "LL/Register.h"
#include "LL/FixedPoint.h"
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
  using ADMUX  = LL::Reg<addr::admux>;
  using ADCSRA = LL::Reg<addr::adcsra>;
  using ADCSRB = LL::Reg<addr::adcsrb>;
  using Data   = LL::Reg<addr::adcl, 12>;
  using ADCL   = LL::Reg<addr::adcl>;
  using ADCH   = LL::Reg<addr::adch>;
  using DIDR0  = LL::Reg<addr::didr0>;

public:
  
  //initialize ADC
  AlwayInline static void init(const ADRefs::T ref = ADRefs::AVcc, 
      bool alignLeft = false,
      bool autoReload = false){

    //Define some variables
    auto Adcsra = LL::RegSet<addr::adcsra>();
    auto mux = LL::RegSet<addr::admux>();

    //Turn on ADC
    PowerManager::turnOnAdc();

    //Set Ref voltage 
    mux.write<7,6>(ref);
    //set alignment
    mux.write<5>(alignLeft);

    //Enble ADC
    Adcsra.write<bit::aden>(1);
    Adcsra.write<bit::adate>(autoReload);
    //set prescale
    Adcsra.write<bit::adps2,bit::adps1,bit::adps0>(ADPrescale::P128);

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
    ADCSRA::write<bit::adsc>(1);
  }

  //Read the output 
  //use when left align is true
  template<unsigned pin>
  AlwayInline static uint8_t read8(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    ADMUX::write<3,2,1,0>(LL::BitSet<4>(actPin));
    while(!ADCSRA::testAndSet<bit::adif>()); 
    //wait for conversion to be done
    
    return ADCH::read().getValue();
  }

  //Read the output
  //Use when left align is false
  template<unsigned pin>
  AlwayInline static LL::Fixed<uint16_t, 10> read(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    ADMUX::write<3,2,1,0>(LL::BitSet<4>(actPin));
    while(!ADCSRA::testAndSet<bit::adif>()); 
    
    return Data::read().getValue();
  }

}; //End of Analog
} //End of Arduino
