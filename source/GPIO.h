#pragma once

#include "templateList.h"
#include "Register.h"
#include "Pin.h"

namespace Arduino{

//
static constexpr unsigned int High = 1;
static constexpr unsigned int Low = 0;
static constexpr unsigned int Output = 1;
static constexpr unsigned int Input = 0;

namespace {

//used to make sure the pins are in the same ports
template<int first, int... others>
struct PinChecker{
  static constexpr auto port = uint8_t(digitalPin<first>::port);
  static constexpr bool check(){
    return port == PinChecker<others...>::port;
  }
};
template<int first>
struct PinChecker<first>{
  static constexpr auto port = uint8_t(digitalPin<first>::port);
};

}//End of Anonymous


template<int pinOne, int... pins>
class GPIO {
  static_assert(PinChecker<pinOne, pins...>::check(), 
      "Pins aren't on the same port");

  using port     = digitalPin<pinOne>;
  using portxReg = LowLevel::Register<LowLevel::Access::rw,
        uint8_t(port::port), 0, 8>;

  using portx    = typename portxReg::template Bit<port::bit,
        digitalPin<pins>::bit...>;

  using ddrxReg  = LowLevel::Register<LowLevel::Access::rw, uint8_t(port::ddr),
        0, 8>;

  using ddrx     = typename ddrxReg::template Bit<port::bit,
        digitalPin<pins>::bit...>;

  using pinList  = typename Meta::makeList<pinOne, pins...>::Value;
  static constexpr uint8_t mask = ddrx::mask;

public:
  //writes value to all pins
  static void writeAll(uint8_t value){
    portx::writeAll(value);
  }

  //write a different value for every pin
  //Please only write 1 or 0. Bad things will happen if you don't.
  template<typename... H>
  static void write(H... h){
    static_assert(sizeof...(h) == sizeof...(pins)+1, 
        "You need the same number of write values and pins");
    portx::write(h...);
  }

  //set all to output
  static void setAllOutput(){
    uint8_t currentValue = ddrxReg::read();
    ddrxReg::wwrite(mask | currentValue);
  }
  
  //set all to input
  static void setAllInput(){
    uint8_t currentValue = ddrxReg::read();
    ddrxReg::wwrite(~mask & currentValue);
  }

  //set all to either an input or output
  //1 is an output
  //0 is an input
  template<typename... T>
  static void setType(T... values){
    static_assert(sizeof...(values) == sizeof...(pins)+1, 
        "You need the same number of write values and pins");
    ddrx::write(values...);
  }
  
  //Read from the GPIO
  //Pass references for ever bit
  template<typename... T>
  static void read(T&... var){
    static_assert(sizeof...(var) == sizeof...(pins)+1, 
        "You need the same number of references and pins");
    portx::read(var...);
  }

};

//specialized GPIO for only 1 GPIO
template<int pin>
class GPIO<pin>{
  using port = digitalPin<pin>; 
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
    portx::write(value&1);
  }
  //reads from the pin
  static uint8_t read(){
    return portx::read();
  }
};

}
