#pragma once
#include "LL/Register.h"
#include "avr/addresses.h"

namespace Arduino{

//Information about the Digital pins
template<uint8_t pin>
struct digitalPin{
  static_assert(pin <= 13, "Not a valid pin");
  using addr = avr::addr;

  static constexpr auto port = (pin < 8)? addr::portd : addr::portb;
  static constexpr auto mask = (pin < 8)? 1<<pin : 1<<(pin-8);
  static constexpr auto bit  = (pin < 8)? pin : pin-8;
  static constexpr auto ddr  = (pin < 8)? addr::ddrd : addr::ddrb;

  using Port = LL::Reg<port>;
  using DDR = LL::Reg<ddr>;
}; //end of Pin<Digital, pin>

//Information about the Analog pins
//Useful for the arduino Mega, I think...
template<uint8_t pin>
struct analogPin{
  static_assert(pin<=7, "Not a valid pin number");
  static constexpr auto muxValue = pin;
}; // end of Pin<Analog, pin>

} //end of Arduino
