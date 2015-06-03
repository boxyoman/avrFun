#pragma once
#include "LL/Register.h"
#include "avr/addresses.h"

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
  using addr = avr::addr;

  static constexpr auto port = (pin < 7)? addr::portd : addr::portb;
  static constexpr auto mask = (pin < 7)? 1<<pin : 1<<(pin-8);
  static constexpr auto bit  = (pin < 7)? pin : pin-8;
  static constexpr auto ddr  = (pin < 7)? addr::ddrd : addr::ddrb;

  using Port = LL::Reg<port>;
  using DDR = LL::Reg<ddr>;
}; //end of Pin<Digital, pin>

//Information about the Analog pins
//Useful for the arduino Mega, I think...
template<uint8_t pin>
struct analogPin{
  static_assert(pin<7, "Not a valid pin number");
  static constexpr auto muxValue = pin;
}; // end of Pin<Analog, pin>

} //end of Arduino
