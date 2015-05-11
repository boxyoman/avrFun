#pragma once

#include "templateList.h"
#include "Register.h"
#include "Bit.h"

namespace Arduino{

//Most of the stuff in this namespace should get put into another file specific 
//to the arduino bbeing used
namespace {
//Get AVR information based on Arduino pin number
template<uint8_t pin>
struct Port{
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

  //generate the write value to write to multiple pins
  template<typename inPins, typename... t>
  static uint8_t getWriteValue(LowLevel::Bit first, t... values) {
    //need to build an assert system
    //assert(first == 1 | first == 0, "Inputs can only be 1 or 0");
    return first<<Port<inPins::Value>::bit 
      | getWriteValue<typename inPins::Next>(values...);
  };
  template<typename inPins>
  static uint8_t getWriteValue(){
    return 0;
  }

  //assign values to the references from a read
  template<typename inPins,typename H, typename... T>
  static void assignRead(uint8_t value, H& head, T&... others){
    uint8_t bit = Port<inPins::Value>::bit;
    head = (value>>bit) & 1;
    assignRead<typename inPins::Next>(value, others...);
  }
  template<typename inPins>
  static void assignRead(uint8_t value){
    return;
  }

  using port = Port<pinOne>;
  using portx = LowLevel::Register<LowLevel::Access::rw, uint8_t(port::port),
        0, 8>;
  using ddrx = LowLevel::Register<LowLevel::Access::rw, uint8_t(port::ddr),
        0, 8>;
  using pinList = typename Meta::makeList<pinOne, pins...>::Value;

public:
  //writes value to all pins
  static void writeAll(LowLevel::Bit value){
    uint8_t mask = maskGen<pinOne, pins...>::mask;
    uint8_t currentValue = portx::read();
    if(value == 0){
      portx::wwrite(~mask & currentValue);
    }else{
      portx::wwrite(mask | currentValue);
    }
  }

  //write a different value for every pin
  //Please only write 1 or 0. Bad things will happen if you don't.
  template<typename... H>
  static void write(H... h){
    static_assert(sizeof...(h) == sizeof...(pins)+1, 
        "You need the same number of write values and pins");
    uint8_t mask = maskGen<pinOne, pins...>::mask;
    uint8_t value = getWriteValue<pinList>(h...);
    uint8_t currentValue = portx::read();
    portx::wwrite((~mask & currentValue) | value);
  }

  //set all to output
  static void setAllOutput(){
    uint8_t mask = maskGen<pinOne, pins...>::mask;
    uint8_t currentValue = portx::read();
    ddrx::wwrite(mask | currentValue);
  }

  //set all to either an input or output
  //1 is an output
  //0 is an input
  template<typename... T>
  static void setType(T... values){
    static_assert(sizeof...(values) == sizeof...(pins)+1, 
        "You need the same number of write values and pins");
    uint8_t mask = maskGen<pinOne, pins...>::mask;
    uint8_t value = getWriteValue<pinList>(values...);
    uint8_t currentValue = ddrx::read();
    ddrx::wwrite((~mask & currentValue) | value);
  }

  //set all to input
  static void setAllInput(){
    uint8_t mask = maskGen<pinOne, pins...>::mask;
    uint8_t currentValue = portx::read();
    ddrx::wwrite(~mask & currentValue);
  }
  
  template<typename... T>
  static void read(T&... var){
    uint8_t value = portx::read();   
    assignRead<pinList>(value, var...);
  }

  //TODO: set input, reading the inputs
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
