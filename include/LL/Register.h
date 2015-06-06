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
*/
/*****************************************************************************/
#pragma once

#include <stdint.h>
#include <limits>
#include "Device.h"
#include "templateList.h"
#include "BitSet.h"
#include "config.h"
#include "Array.h"
#include "Access.h"

namespace LL{

template<
  std::size_t addr, 
  typename T = Device::Word,
  typename mut_t = Access::rw, //TODO: use a contrainer 
  std::size_t width = std::numeric_limits<T>::digits, 
  std::size_t offset = 0 
>
class Reg {
  using reg_t = volatile T*;
  static constexpr unsigned int size = std::numeric_limits<T>::digits;

public:

  static_assert(!std::numeric_limits<T>::is_signed, "Needs to be unsigned");
  static_assert(std::numeric_limits<T>::is_integer, "Not an integer");
  static_assert(width > 0, "Not a positive width");
  static_assert(offset >= 0, "Negative offset");
  static_assert(width + offset <= std::numeric_limits<T>::digits, 
      "width overflow");

  AlwayInline static auto read(){
    return mut_t::template read<addr, offset, width, T>();
  }

  template<int... bits>
  AlwayInline static void write(LL::BitSet<width> val){
    //TODO: write a better assert message
    static_assert((sizeof...(bits) > 0 && width == size) | 
        (sizeof...(bits) == 0), "Error"); 

    if(sizeof...(bits) == 0){
      mut_t::template write<addr, offset, width, T>(val);
    }else{
      array<std::size_t, sizeof...(bits)> bit = {bits...};
      auto writeValue = BitSet<size>();
      for (std::size_t i = 0; i < sizeof...(bits); ++i){
        writeValue[bit[i]] = val[i];
      }
      mut_t::template write<addr, offset, width, T>(writeValue);
    }
  }

  //Call when destructively writing
  template<int... bits>
  AlwayInline static void wwrite(LL::BitSet<width> val){
    static_assert((sizeof...(bits) > 0 & width == size) | 
        (sizeof...(bits) == 0), "Error"); 

    if(sizeof...(bits) == 0){
      mut_t::template wwrite<addr, offset, width, T>(val);
    }else{
      array<std::size_t, sizeof...(bits)> bit = {bits...};
      auto writeValue = BitSet<size>();
      for (std::size_t i = 0; i < sizeof...(bits); ++i){
        writeValue[bit[i]] = val[i];
      }
      mut_t::template wwrite<addr, offset, width, T>(writeValue);
    }
  }

  template<std::size_t bit>
  static bool testAndSet(){
    bool val = mut_t::read()[bit];
    if(val == 1){
      mut_t::write<bit>(1);
    }
    return val;
  }

}; //End of class Register<mux_t, addr, offset, width, T>

}//End of namespace Register
