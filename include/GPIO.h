#pragma once

#include "LL/Register.h"
#include "Pin.h"

namespace Arduino{

//Make life easier
static constexpr bool High = 1;
static constexpr bool Low = 0;
static constexpr bool Output = 1;
static constexpr bool Input = 0;

namespace {

//used to make sure the pins are in the same port
template<unsigned int first, unsigned int... others>
struct PinChecker{
  static constexpr auto port = uint8_t(digitalPin<first>::port);
  static constexpr bool check(){
    return port == PinChecker<others...>::port;
  }
};
template<unsigned int first>
struct PinChecker<first>{
  static constexpr auto port = uint8_t(digitalPin<first>::port);
};

}//End of Anonymous


template<unsigned int pinOne, unsigned int... pins>
class GPIO {
  static_assert(PinChecker<pinOne, pins...>::check(), 
      "Pins aren't on the same port");
  static constexpr auto size = sizeof...(pins)+1;

  //I'm lazy
  template<std::size_t pinNum>
  using pin = digitalPin<pinNum>;

  template<typename... T>
  static uint8_t getBitSetValue(bool h, T... t){
    return h<<sizeof...(t) | getBitSetValue(t...);
  }
  static auto getBitSetValue(){
    return 0;
  }

  using port  = digitalPin<pinOne>;
  using portx = LL::Reg<port::port>;
  using ddrx  = LL::Reg< port::ddr>;

public:
  //writes value to all pins
  AlwayInline static void writeAll(bool value){
    auto out = LL::BitSet<size>();
    if(value){
      out.set();
    }else{
      out.clear();
    }
    portx::template write<port::bit, pin<pins>::bit...>(out);
  }

  AlwayInline static void writeValue(uint8_t value){
    auto bits = LL::BitSet<size>(value);
    portx::template write<port::bit, pin<pins>::bit...>(bits);
  }

  //write a different value for every pin
  //Please only write 1 or 0. Bad things will happen if you don't.
  template<typename... H>
  AlwayInline static void write(H... h){
    static_assert(sizeof...(h) == size, 
        "You need the same number of write values and pins");
    portx::template 
      write<port::bit, pin<pins>::bit...>(getBitSetValue(h...));
  }

  template<unsigned n>
  AlwayInline static void write(LL::BitSet<n> a){
    portx::template write<port::bit, pin<pins>::bit...>(a);
  }

  //set all to output
  AlwayInline static void setAllOutput(){
    auto out = LL::BitSet<size>();
    ddrx::template write<pin<pinOne>::bit, pin<pins>::bit...> (out.set());
  }
  
  //set all to input
  AlwayInline static void setAllInput(){
    auto out = LL::BitSet<size>();
    ddrx::template write<pin<pinOne>::bit, pin<pins>::bit...> (out);
  }

  //set all to either an input or output
  template<typename... T>
  AlwayInline static void setType(T... values){
    static_assert(sizeof...(values) == size, 
        "You need the same number of write values and pins");
    ddrx::template 
      write<pin<pinOne>::bit, pin<pins>::bit...> (getBitSetValue(values...));
  }
  
  //Read from the GPIO
  //Pass references for ever bit
  AlwayInline static LL::BitSet<size> read(){
    return portx::read();
  }

};

//specialized GPIO for only 1 GPIO
template<unsigned int pin>
class GPIO<pin>{
  using port = digitalPin<pin>; 
  using portx = LL::Reg<port::port, 1, port::bit>;
  using ddrx = LL::Reg<port::ddr, 1, port::bit>;
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
