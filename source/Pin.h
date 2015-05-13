#pragma once
#include "Register.h"

namespace Arduino{

//Type of pins use on the Arduino
enum class PinTypes{
  Digital,
  Analog
};

//Information about the Digital pins
template<uint8_t pin>
struct digitalPin{
  static_assert(pin <= 13, "Not a valid pin");
  
  //AVR port addresses
  enum class PortX : uint8_t{
    B = 0x25,
    D = 0x2b,
  };

  //AVR DDR addresses
  enum class DDRx : uint8_t{
    B = 0x24,
    D = 0x2a,
  };

  static constexpr auto port = uint8_t((pin < 7)? PortX::D : PortX::B);
  static constexpr auto mask = (pin < 7)? 1<<pin : 1<<(pin-8);
  static constexpr auto bit  = (pin < 7)? pin : pin-8;
  static constexpr auto ddr  = uint8_t((pin < 7)? DDRx::D : DDRx::B);

  using Port = LL::Register<LL::Access::wr, port>;
  using PortBit = typename Port::template Bit<bit>;
  using DDR = LL::Register<LL::Access::wr, ddr>;
  using DDRBit = typename DDR::template Bit<bit>;
}; //end of Pin<Digital, pin>

//Information about the Analog pins
//Useful for the arduino Mega, I think...
template<uint8_t pin>
struct analogPin{
  static_assert(pin<7, "Not a valid pin number");
  static constexpr auto muxValue = pin;
  static constexpr auto admux    = 0x7c;
  static constexpr auto acsr     = 0x50;
  static constexpr auto adcsra   = 0x7a;
  static constexpr auto adcsrb   = 0x7b;
  static constexpr auto adch     = 0x79;
  static constexpr auto adcl     = 0x78;
}; // end of Pin<Analog, pin>

} //end of Arduino
