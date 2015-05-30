#pragma once

#include "LL/Register.h"
#include "PowerManager.h"
#include "Pin.h"
#include "LL/RegSet.h"

namespace Arduino{


namespace ADRefs{
  using T = LL::BitSet<2>;
  constexpr auto Vref = T(0);
  constexpr auto External = T(1);
  constexpr auto Internal = T(3);
}
namespace ADPrescale{
  using T = LL::BitSet<3>;
  constexpr auto P2   = T(1);
  constexpr auto P4   = T(2);
  constexpr auto P8   = T(3);
  constexpr auto P16  = T(4);
  constexpr auto P32  = T(5);
  constexpr auto P64  = T(6);
  constexpr auto P128 = T(7);
}

class Analog {
  enum{
    adcl   = 0x78,
    adch   = 0x79,
    adcsra = 0x7a,
    adcsrb = 0x7b,
    admux  = 0x7c,
    didr0  = 0x7e,
    acsr   = 0x50,
  };

  enum{
#define ADPS 2,1,0
    ADIE = 3,
    ADIF,
    ADATE,
    ADSC,
    ADEN,
  };

  //useful registers
  using ADMUX = LL::Register<LL::Access::wr, admux>;
  using MUX = typename ADMUX::template Bit<3,2,1,0>;

  using ADCSRAReg = LL::Register<LL::Access::wr, adcsra>;
  //using ADEN = typename ADCSRA::template Bit<7>;
  using ADSCReg = typename ADCSRAReg::template Bit<6>;
  //using ADATE = typename ADCSRA::template Bit<5>;
  using ADIFReg = typename ADCSRAReg::template Bit<4>;
  //using ADIE = typename ADCSRA::template Bit<3>;
  //using ADPS = typename ADCSRA::template Bit<2,1,0>;

  using Data = LL::Register<LL::Access::wr, adcl, 0, 16, uint16_t>;
  using ADCL = LL::Register<LL::Access::wr, adcl>;
  using ADCH = LL::Register<LL::Access::wr, adch>;

  using ADCSRB = LL::Register<LL::Access::wr, adcsrb>;
  using ACME = typename ADCSRB::template Bit<6>;
  using ADTS = typename ADCSRB::template Bit<2,1,0>;

  using DIDR0 = LL::Register<LL::Access::wr, didr0>;
  using ADC0D = typename DIDR0::template Bit<0>;
  using ADC1D = typename DIDR0::template Bit<1>;
  using ADC2D = typename DIDR0::template Bit<2>;
  using ADC3D = typename DIDR0::template Bit<3>;
  using ADC4D = typename DIDR0::template Bit<4>;
  using ADC5D = typename DIDR0::template Bit<5>;

  using ACSR = LL::Register<LL::Access::wr, acsr>;
  using ACD = ACSR::template Bit<7>;
public:
  
  //initialize ADC
  AlwayInline static void init(const ADRefs::T ref = ADRefs::Vref, 
      const ADPrescale::T prescale = ADPrescale::P2,
      bool alignLeft = false){

    //Turn on ADC
    PowerManager::turnOnAdc();

    //Set Ref voltage 
    auto test = LL::RegSet<admux>();
    test.write<7,6>(ref);
    //set alignment
    test.write<5>(alignLeft);

    //Enble ADC
    auto ADCSRA = LL::RegSet<adcsra>();
    ADCSRA.write<ADEN>(1);
    //set prescale
    ADCSRA.write<ADPS>(prescale);
  }

  AlwayInline static void init(
      const ADPrescale::T prescale,
      const ADRefs::T ref = ADRefs::Vref, 
      bool alignLeft = false){
    init(ref, prescale, alignLeft);
  }

  AlwayInline static void init(bool alignLeft,
      const ADPrescale::T prescale = ADPrescale::P2,
      const ADRefs::T ref = ADRefs::Vref 
      ){
    init(ref, prescale, alignLeft);
  }

  //Set pin to an adc input
  template<unsigned pin>
  AlwayInline static void setToADC(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    auto didr = LL::RegSet<didr0>();
    didr.write<actPin>(1);
  }

  //Read the output 
  //use when left align is true
  template<unsigned pin>
  AlwayInline static uint8_t read8(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    MUX::write(LL::BitSet<4>(actPin));
    ADSCReg::write(1);
    while(!ADIFReg::testAndSet()); //wait for conversion to be done
    ADCL::read();
    return ADCH::read();
  }

  //Read the output
  //Use when left align is false
  template<unsigned pin>
  AlwayInline static uint16_t read(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    MUX::write(LL::BitSet<4>(actPin));
    ADSCReg::write(1);
    while(!ADIFReg::testAndSet()); //wait for conversion to be done
    
    return Data::read();
  }

}; //End of Analog
} //End of Arduino
