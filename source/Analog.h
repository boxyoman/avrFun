#pragma once

#include "Device.h"
#include "Register.h"
#include "PowerManager.h"
#include "Pin.h"

namespace Arduino{

namespace{

//Make life easy
using Access = LL::Access;
template<Access mut_t, 
  unsigned int addr, 
  int offset = 0, 
  int width = std::numeric_limits<Device::Word>::digits, 
  typename T = Device::Word
  >
using Reg = LL::Register<mut_t, addr, offset, width, T>;
}//End of Anonymous namespace

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
  using ADMUX = Reg<Access::wr, admux>;
  using REFS = typename ADMUX::template Bit<7,6>;
  using ADLAR = typename ADMUX::template Bit<5>;
  using MUX = typename ADMUX::template Bit<3,2,1,0>;

  using ADCSRA = LL::Register<Access::wr, adcsra>;
  using ADEN = typename ADCSRA::template Bit<7>;
  using ADSC = typename ADCSRA::template Bit<6>;
  using ADATE = typename ADCSRA::template Bit<5>;
  using ADIF = typename ADCSRA::template Bit<4>;
  using ADIE = typename ADCSRA::template Bit<3>;
  using ADPS = typename ADCSRA::template Bit<2,1,0>;

  using Data = Reg<Access::wr, adcl, 0, 16, uint16_t>;
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

public:
  AlwayInline static void init(){
    PowerManager::turnOnAdc();
    //Enble ADC
    ADEN::write(1);
    //Set Ref voltage
    REFS::write(LL::BitSet<2>(1));
    //Align to the right
    ADLAR::write(1);
    //set prescale
    ADPS::write(LL::BitSet<3>(1));
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

  template<unsigned pin>
  AlwayInline static uint16_t read8(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    MUX::write(LL::BitSet<4>(actPin));
    ADSC::write(1);
    while(!ADIF::testAndSet()); //wait for conversion to be done
    return ADCH::read();
  }

  template<unsigned pin>
  AlwayInline static uint16_t read(){
    constexpr auto actPin = analogPin<pin>::muxValue;
    MUX::write(LL::BitSet<4>(actPin));
    ADSC::write(1);
    while(!ADIF::read()[0]); //wait for conversion to be done
    ADIF::write(1); //clear the flag
    return Data::read();
  }

}; //End of Analog
} //End of Arduino
