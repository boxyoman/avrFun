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
    reg_t device = reinterpret_cast<reg_t>(addr);
    *device = (*device & ~mask) | ((value << offset) & mask);
  }

  //Called when writing has only write access
  static void wwrite(int value){
    reg_t device = reinterpret_cast<reg_t>(addr);
    *device = (value & mask) << offset;
  }

  template<typename ...bits>
  class Bit{
    static_assert(width == std::numeric_limits<T>::digits, 
        "Width must be width of register when using Bit");
    static void writeAll(Bit value){

    }
  };
}; //End of class Register<mux_t, addr, offset, width, T>


}//End of namespace Register

