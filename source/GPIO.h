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
  using portxReg = LL::Register<LL::Access::rw, uint8_t(port::port)>;

  using portx    = typename portxReg::template Bit<port::bit,
        digitalPin<pins>::bit...>;

  using ddrxReg  = LL::Register<LL::Access::rw, uint8_t(port::ddr)>;

  using ddrx     = typename ddrxReg::template Bit<port::bit,
        digitalPin<pins>::bit...>;

  using pinList  = typename Meta::makeList<pinOne, pins...>::Value;
  static constexpr uint8_t mask = ddrx::mask;

public:
  //writes value to all pins
  AlwayInline static void writeAll(uint8_t value){
    portx::writeAll(value);
  }

  AlwayInline static void writeValue(uint8_t value){
    auto bits = LL::BitSet<sizeof...(pins)+1>(value);
    portx::write(bits);
  }

  //write a different value for every pin
  //Please only write 1 or 0. Bad things will happen if you don't.
  template<typename... H>
  AlwayInline static void write(H... h){
    static_assert(sizeof...(h) == sizeof...(pins)+1, 
        "You need the same number of write values and pins");
    portx::write(h...);
  }

  //set all to output
  AlwayInline static void setAllOutput(){
    uint8_t currentValue = ddrxReg::read();
    ddrxReg::wwrite(mask | currentValue);
  }
  
  //set all to input
  AlwayInline static void setAllInput(){
    uint8_t currentValue = ddrxReg::read();
    ddrxReg::wwrite(~mask & currentValue);
  }

  //set all to either an input or output
  //1 is an output
  //0 is an input
  template<typename... T>
  AlwayInline static void setType(T... values){
    static_assert(sizeof...(values) == sizeof...(pins)+1, 
        "You need the same number of write values and pins");
    ddrx::write(values...);
  }
  
  //Read from the GPIO
  //Pass references for ever bit
  template<typename... T>
  AlwayInline static void read(T&... var){
    static_assert(sizeof...(var) == sizeof...(pins)+1, 
        "You need the same number of references and pins");
    portx::read(var...);
  }

};

//specialized GPIO for only 1 GPIO
template<int pin>
class GPIO<pin>{
  using port = digitalPin<pin>; 
  using portx = LL::Register<LL::Access::rw, uint8_t(port::port), 
        port::bit, 1>;
  using ddrx = LL::Register<LL::Access::rw, uint8_t(port::ddr), 
        port::bit, 1>;
public:
  //sets the pin to an input
  AlwayInline static void setInput(){
    ddrx::write(0);
  }
  //sets the pin to an output
  AlwayInline static void setOuput(){
    ddrx::write(1);
  }

  //writes to the pin
  AlwayInline static void write(uint8_t value){
    portx::write(value&1);
  }
  //reads from the pin
  AlwayInline static uint8_t read(){
    return portx::read();
  }
};

}
