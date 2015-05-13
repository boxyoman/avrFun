/*****************************************************************************/
/*
 * The idea for this was stolen from: 
 *   https://github.com/kensmith/cppmmio
 *
 * template<typename mut_t, unsigned int addr, int offset, int width>
 * struct Register
 *
 *  mut_t:
 *    this defines the access of the register options include:
 *      ro - read only
 *      wo - write only
 *      rw - read write
 *      wr - write read (for those of you with dyslexia or a bad memory)
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
 *    using LED  = Register<Access::rw, 0x2a, 0, 1>;
 *    LED::write(1); //turn the LED on
 *
 *    using BitMan = Register<Access::wr, 0x25>::template Bit<3,2,1>;
 *    BitMan::write(0,1,0); // write a 0 to bit 3
 *                          //       a 1 to bit 2
 *                          //   and a 0 to bit 1
 *
 *    //For quick access
 *    RegBit<0x12, 3, 2, 1>::write(0,1,0); //same as before
*/
/*****************************************************************************/
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
  wr,
};

template<Access mut_t, 
  unsigned int addr, 
  int offset = 0, 
  int width = std::numeric_limits<Device::Word>::digits, 
  typename T = Device::Word
  >
class Register {
  using reg_t = volatile T*;
  static constexpr int deviceWidth = std::numeric_limits<T>::digits;
  static constexpr T generate_mask(){
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

  //a class for easy simultaneous bit manipulations
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
    static T getWriteValue(h first, t... values) {
      static_assert(std::numeric_limits<h>::is_integer, 
          "Write values must be integers");
      return (1&first)<<inBit::Value 
        | getWriteValue<typename inBit::Next>(values...);
    };
    template<typename inPins>
    static T getWriteValue(){
      return 0;
    }

    //assign values to the references from a read
    template<typename inPins,typename H, typename... t>
    static void assignRead(T value, H& head, t&... others){
      T bit = inPins::Value;
      head = (value>>bit) & 1;
      assignRead<typename inPins::Next>(value, others...);
    }
    template<typename inPins>
    static void assignRead(T value){
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
      T currentValue = (mut_t != Access::ro) ? Register::read() : 0;
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
      T value = Register::read();   
      assignRead<bitList>(value, var...);
    }

  }; //End of class Bit<bits...>
}; //End of class Register<mux_t, addr, offset, width, T>

template<Device::Word addr, Device::Word... bits>
using RegBit = typename Register<Access::rw, addr>:: template Bit<bits...>;

}//End of namespace Register
