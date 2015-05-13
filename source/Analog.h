#pragma once

#include "Device.h"
#include "Register.h"
#include "PowerManager.h"
#include "Pin.h"

namespace Arduino{

namespace{

//Make life easy
using Access = LowLevel::Access;
template<Access mut_t, 
  unsigned int addr, 
  int offset = 0, 
  int width = std::numeric_limits<Device::Word>::digits, 
  typename T = Device::Word
  >
using Reg = LowLevel::Register<mut_t, addr, offset, width, T>;
}//End of Anonymous namespace

template<int pin>
class Analog {
  using device = analogPin<pin>;
  static constexpr auto actPin = device::muxValue;

  //useful registers
  using chSelect = Reg<Access::wr, device::admux, 0, 4>;
  using bitJustify = Reg<Access::wr, device::admux, 5, 1>;
  using Enable = Reg<Access::wr, device::adcsra, 7, 1>;
  using Start = Reg<Access::wr, device::adcsra, 6, 1>;
  using IntrFlag = Reg<Access::wr, device::adcsra, 4, 1>;

  using Data = Reg<Access::wr, device::adcl, 0, 16, uint16_t>;
public:
  static void init(){
    Enable::write(1);
    bitJustify::write(0);
    PowerManager::turnOnAdc();
  }
  static uint16_t read(){
    chSelect::write(actPin);
    Start::write(1);
    while(!Start::read()); //wait for conversion to be done
    IntrFlag::write(1); //clear the flag
    return Data::read();
  }
}; //End of Analog
} //End of Arduino
