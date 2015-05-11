#include <stdint.h>

namespace Device{
  using Word = uint8_t;
}

namespace Arduino{

enum class PinTypes{
  Digital,
  Analog
};

template<PinTypes pinType, uint8_t pin>
struct Pin{};

template<uint8_t pin>
struct Pin<PinTypes::Digital, pin>{
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

  static const auto port = (pin < 7)? PortX::D : PortX::B;
  static const auto mask = (pin < 7)? uint8_t(1<<pin) : 1<<(pin-8);
  static const auto bit  = (pin < 7)? pin : pin-8;
  static const auto ddr  = (pin < 7)? DDRx::D : DDRx::B;
}; //end of port

template<uint8_t pin>
struct Pin<PinTypes::Analog, pin>{

};

}//end of Arduino

