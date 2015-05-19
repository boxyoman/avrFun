#pragma once

#include "Device.h"
#include "Register.h"
#include "PowerManager.h"
#include "Pin.h"

namespace Arduino{

namespace ADRefs{
  constexpr auto Vref = LL::BitSet<2>(0);
  constexpr auto External = LL::BitSet<2>(1);
  constexpr auto Internal = LL::BitSet<2>(3);
}
namespace ADPrescale{
  constexpr auto P2   = LL::BitSet<2>(1);
  constexpr auto P4   = LL::BitSet<2>(2);
  constexpr auto P8   = LL::BitSet<2>(3);
  constexpr auto P16  = LL::BitSet<2>(4);
  constexpr auto P32  = LL::BitSet<2>(5);
  constexpr auto P64  = LL::BitSet<2>(6);
  constexpr auto P128 = LL::BitSet<2>(7);
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

  //useful registers
  using ADMUX = LL::Register<LL::Access::wr, admux>;
  using MUX = typename ADMUX::template Bit<3,2,1,0>;

  using ADCSRA = LL::Register<LL::Access::wr, adcsra>;
  using ADEN = typename ADCSRA::template Bit<7>;
  using ADSC = typename ADCSRA::template Bit<6>;
  using ADATE = typename ADCSRA::template Bit<5>;
  using ADIF = typename ADCSRA::template Bit<4>;
  using ADIE = typename ADCSRA::template Bit<3>;
  using ADPS = typename ADCSRA::template Bit<2,1,0>;

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
  AlwayInline static void init(const LL::BitSet<2> ref = ADRefs::Vref, 
      const LL::BitSet<3> prescale = ADPrescale::P2,
      bool alignLeft = false){

    ACD::write(1);
    //Turn on ADC
    PowerManager::turnOnAdc();
    //Enble ADC
    ADEN::write(1);
    //Set Ref voltage and alignment
    using RefAlign = typename ADMUX::template Bit<7,6,5>;
    RefAlign::write(ref+LL::BitSet<1>(alignLeft));
    //set prescale
    ADPS::write(prescale);
  }

  //Set pin to an adc input
  template<unsigned pin>
  AlwayInline static void setToADC(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    switch (actPin){
      case 0:
        ADC0D::write(1);
        break;
      case 1:
        ADC1D::write(1);
        break;
      case 2:
        ADC2D::write(1);
      case 3:
        ADC3D::write(1);
        break;
      case 4:
        ADC4D::write(1);
        break;
      case 5:
        ADC5D::write(1);
        break;
    }
  }

  //Read the output 
  //use when left align is true
  template<unsigned pin>
  AlwayInline static uint8_t read8(){
    auto actPin = pin;
    //constexpr auto actPin = analogPin<pin>::muxValue;
    MUX::write(LL::BitSet<4>(actPin));
    
    //start
    ADSC::write(1);
    while(!ADIF::testAndSet()); //wait for conversion to be done
    ADCL::read();
    return ADCH::read();
  }

  //Read the output
  //Use when left align is false
  template<unsigned pin>
  AlwayInline static uint16_t read(){
    auto actPin = pin;
    //constexpr auto actPin = analogPin<pin>::muxValue;
    MUX::write(LL::BitSet<4>(actPin));
    ADSC::write(1);
    while(!ADIF::read()[0]); //wait for conversion to be done
    ADIF::write(1); //clear the flag

    return Data::read();
  }

}; //End of Analog
} //End of Arduino
