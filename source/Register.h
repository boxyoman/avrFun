/*******************************************************************/
/*
 * The idea for this was stolen from: 
 *   https://github.com/kensmith/cppmmio
 *
 * template<typename mut_t, uint8_t addr, int offset, int width>
 * struct Register
 *
 *  mut_t:
 *    this defines the access of the register options include:
 *      ro - read only
 *      rw - read write
 *      wo - write only
 *
 *  addr:
 *    The address of the register
 *
 *  offset:
 *    The offset of the register
 *
 *  width:
 *    The width of the register
 *
 * example: 
 *    using LED  = Register<rw_t, 0xffff0000, 0, 1>;
 *    LED::write(1); //turn the LED on
*/
/******************************************************************/
#pragma once

#include <stdint.h>
#include <limits>
#include "Device.h"
#include "templateList.h"

namespace LowLevel{

enum class Access {
  ro,
  wo,
  rw,
};

template<Access mut_t, 
  unsigned int addr, 
  int offset, 
  int width, 
  typename T = Device::Word>
class Register {
  using reg_t = volatile T*;
  static const int deviceWidth = std::numeric_limits<T>::digits;
  static constexpr uint8_t generate_mask(){
    return (width>=deviceWidth)? ~0 : ((1 << width) - 1) << offset;
  }
  static constexpr T mask = generate_mask();
public:

  static_assert(!std::numeric_limits<T>::is_signed, "Needs to be unsigned");
  static_assert(std::numeric_limits<T>::is_integer, "Not an integer");
  static_assert(width > 0, "Not a positive width");
  static_assert(offset >= 0, "Negative offset");
  static_assert(width + offset <= std::numeric_limits<T>::digits, 
      "width overflow");

  static T read(){
    static_assert(mut_t != Access::wo, "Write only register");
    reg_t device = reinterpret_cast<reg_t>(addr);
    return (*device & mask) >> offset;
  }

  static void write(T val){
    static_assert(mut_t != Access::ro, "Read only register");
    if(mut_t == Access::rw)
      rwrite(val);
    else
      wwrite(val);
  }
  
  //Called when writing has both read and write access
  static void rwrite(int value){
    static_assert(mut_t != Access::ro, 
        "Trying to write to a readonly register");
    reg_t device = reinterpret_cast<reg_t>(addr);
    *device = (*device & ~mask) | ((value << offset) & mask);
  }

  //Called when writing has only write access
  static void wwrite(int value){
    static_assert(mut_t != Access::ro, 
        "Trying to write to a readonly register");
    reg_t device = reinterpret_cast<reg_t>(addr);
    *device = (value & mask) << offset;
  }

  //a class for direct simultaneous bit manipulations
  template<int ...bits>
  class Bit{
    static_assert(width == std::numeric_limits<T>::digits, 
        "Width must be width of register when using Bit");

    //Generates Mask for reading 
    template<int pos1, int... others>
    struct maskGen{
      static constexpr T mask = 1<<pos1 | maskGen<others...>::mask;
    };
    template<int pos>
    struct maskGen<pos>{
      static constexpr T mask = 1<<pos;
    };

    //generate the write value to write to multiple pins
    //inBit should be a List of the bits
    template<typename inBit, typename h, typename... t>
    static uint8_t getWriteValue(h first, t... values) {
      return (1&first)<<inBit::Value 
        | getWriteValue<typename inBit::Next>(values...);
    };
    template<typename inPins>
    static uint8_t getWriteValue(){
      return 0;
    }

    //assign values to the references from a read
    template<typename inPins,typename H, typename... t>
    static void assignRead(uint8_t value, H& head, t&... others){
      uint8_t bit = inPins::Value;
      head = (value>>bit) & 1;
      assignRead<typename inPins::Next>(value, others...);
    }
    template<typename inPins>
    static void assignRead(uint8_t value){
      return;
    }

    //A list of the bits
    using bitList = typename Meta::makeList<bits...>::Value;

  public:
    //Mask used to show what bits are being used in the register
    static constexpr T mask = maskGen<bits...>::mask;

    //write the same value to every bit
    static void writeAll(T value){
      static_assert(mut_t != Access::ro, 
          "Trying to write to a readonly register");
      value &= 1;
      T currentValue = (mut_t != Access::ro) ? read() : 0;
      if(value == 1){
        Register::wwrite(mask | currentValue);
      }else{
        Register::wwrite(~mask & currentValue);
      }
    }

    //write different values to the bits
    template<typename... H>
    static void write(H... values){
      static_assert(sizeof...(values) == sizeof...(bits), 
          "You need the same number of write values and bits");
      static_assert(mut_t != Access::ro, 
          "Trying to write to a readonly register");
      if(mut_t == Access::wo){
        Bit::wwrite(values...);
      }else{
        Bit::rwrite(values...);
      }
    }
    
    //write different values to the bits
    template<typename... H>
    static void rwrite(H... values){
      static_assert(sizeof...(values) == sizeof...(bits), 
          "You need the same number of write values and bits");
      static_assert(mut_t != Access::ro, 
          "Trying to write to a readonly register");
      T value = getWriteValue<bitList>(values...);
      T currentValue = (mut_t != Access::ro) ? Register::read() : 0;
      Register::wwrite((~mask & currentValue) | value);
    }

    //write different values to the bits overriding all bits
    template<typename... H>
    static void wwrite(H... values){
      static_assert(sizeof...(values) == sizeof...(bits), 
          "You need the same number of write values and bits");
      static_assert(mut_t != Access::ro, 
          "Trying to write to a readonly register");
      T value = getWriteValue<bitList>(values...);
      Register::wwrite(value);
    }
    
    template<typename... H>
    static void read(H&... var){
      static_assert(sizeof...(var) == sizeof...(bits), 
          "You need the same number of references and pins");
      static_assert(mut_t != Access::wo, 
          "Trying to read from a write only register.");
      uint8_t value = Register::read();   
      assignRead<bitList>(value, var...);
    }

  }; //End of class Bit<bits...>
}; //End of class Register<mux_t, addr, offset, width, T>


}//End of namespace Register
