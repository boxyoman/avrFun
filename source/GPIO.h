#include "Register.h"

namespace Arduino{

namespace {

//Get AVR information based on Arduino pin number
template<uint8_t pin>
struct Port{
  static_assert(pin <= 13, "Not a valid pin");
  
  //AVR port
  enum class PortX : uint8_t{
    B = 0x25,
    D = 0x2b,
  };

  enum class DDRx : uint8_t{
    B = 0x24,
    D = 0x2a,
  };

  static const auto port = (pin < 7)? PortX::D : PortX::B;
  static const auto mask = (pin < 7)? uint8_t(1<<pin) : 1<<(pin-8);
  static const auto bit  = (pin < 7)? pin : pin-8;
  static const auto ddr  = (pin < 7)? DDRx::D : DDRx::B;

};

//used to make sure the pins are in the same ports
template<int first, int... others>
struct PortChecker{
  static const auto port = uint8_t(Port<first>::port);
  static constexpr bool check(){
    return port == PortChecker<others...>::port;
  }
};

//Specialized PortChecker
template<int first>
struct PortChecker<first>{
  static const auto port = uint8_t(Port<first>::port);
};


}//End of Anonymous

template<int pinOne, int... pins>
class GPIO {
  static_assert(PortChecker<pinOne, pins...>::check(), 
      "pins aren't on the same port");

  //Generates Mask for reading and writting
  template<int first, int... others>
  struct maskGen{
    static constexpr auto pinMask = uint8_t(Port<first>::mask);
    static constexpr auto mask = pinMask | maskGen<others...>::mask;
  };
  template<int first>
  struct maskGen<first>{
    static constexpr auto mask = uint8_t(Port<first>::mask);
  };

  using port = Port<pinOne>;
  using portx = LowLevel::Register<LowLevel::Access::rw, uint8_t(port::port),
        0, 8>;

public:
  //writes value to all pins
  static void write(uint8_t value){
    uint8_t mask = maskGen<pinOne, pins...>::mask;
    uint8_t currentValue = portx::read();
    if(value == 0){
      portx::wwrite(~mask & currentValue);
    }else{
      portx::wwrite(mask | currentValue);
    }
  }
  //set all to output
  static void setOutput(){
    uint8_t mask = maskGen<pinOne, pins...>::mask;
    uint8_t currentValue = portx::read();
    portx::wwrite(mask | currentValue);
  }
};

//specialized GPIO for only 1 GPIO
template<int pin>
class GPIO<pin>{
  using port = Port<pin>; 
  using portx = LowLevel::Register<LowLevel::Access::rw, uint8_t(port::port), 
        port::bit, 1>;
  using ddrx = LowLevel::Register<LowLevel::Access::rw, uint8_t(port::ddr), 
        port::bit, 1>;
public:
  //sets the pin to an input
  static void setInput(){
    ddrx::write(0);
  }
  //sets the pin to an output
  static void setOuput(){
    ddrx::write(1);
  }

  //writes to the pin
  static void write(uint8_t value){
    portx::write(value);
  }
  //reads from the pin
  static uint8_t read(){
    return portx::read();
  }
};

}
