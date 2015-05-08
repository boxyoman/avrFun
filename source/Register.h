
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

namespace LowLevel{

enum class Access {
  ro,
  wo,
  rw,
};

template<Access mut_t, 
  uint8_t addr, 
  int offset, 
  int width, 
  typename t = uint8_t>
class Register {
  using reg_t = volatile t*;
  static constexpr reg_t device = reinterpret_cast<reg_t>(addr);
  static const int deviceWidth = std::numeric_limits<t>::digits;
  static constexpr uint8_t generate_mask(){
    return (width>=deviceWidth)? ~0 : ((1 << width) - 1) << offset;
  }
  static constexpr t mask = generate_mask();
public:

  static_assert(!std::numeric_limits<t>::is_signed, "Needs to be unsigned");
  static_assert(std::numeric_limits<t>::is_integer, "Not an integer");
  static_assert(width > 0, "Not a positive width");
  static_assert(offset >= 0, "Negative offset");
  static_assert(width + offset <= std::numeric_limits<t>::digits, 
      "width overflow");

  static uint8_t read(){
    static_assert(mut_t != Access::wo, "Write only register");
    return (*reinterpret_cast<reg_t>(addr) & generate_mask()) 
            >> offset;
  }

  static void write(uint8_t val){
    static_assert(mut_t != Access::ro, "Read only register");
    if(mut_t == Access::rw)
      rwrite(val);
    else
      wwrite(val);
  }
  
  //Called when writing has both read and write access
  static void rwrite(int value){
    *device = (*device & ~mask) | ((value << offset) & mask);
  }

  //Called when writing has only write access
  static void wwrite(int value){
    *device = (value & mask) << offset;
  }
private:

}; //End of class Register

}//End of namespace Register

