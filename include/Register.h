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
#include "BitSet.h"
#include "config.h"
#include "Array.h"

namespace LL{

enum class Access {
  ro,
  wo,
  rw,
  wr,
};

template<Access mut_t, 
  unsigned int addr, 
  unsigned int offset = 0, 
  unsigned int width = std::numeric_limits<Device::Word>::digits, 
  typename T = Device::Word
  >
class Register {
  using reg_t = volatile T*;
  static constexpr unsigned int deviceWidth = std::numeric_limits<T>::digits;
  AlwayInline static constexpr T generate_mask(){
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

  AlwayInline static T read(){
    static_assert(mut_t != Access::wo, "Write only register");
    reg_t device = reinterpret_cast<reg_t>(addr);
    return (*device & mask) >> offset;
  }

  AlwayInline static void write(T val){
    static_assert(mut_t != Access::ro, "Read only register");
    if(mut_t == Access::rw)
      rwrite(val);
    else
      wwrite(val);
  }
  
  //Called when writing has both read and write access
  AlwayInline static void rwrite(T value){
    static_assert(mut_t != Access::ro, 
        "Trying to write to a readonly register");
    reg_t device = reinterpret_cast<reg_t>(addr);
    *device = (*device & ~mask) | ((value << offset) & mask);
  }

  //Called when writing has only write access
  AlwayInline static void wwrite(T value){
    static_assert(mut_t != Access::ro, 
        "Trying to write to a readonly register");
    reg_t device = reinterpret_cast<reg_t>(addr);
    *device = (value & mask) << offset;
  }

  // A class for easy simultaneous bit manipulations
  // Order from MSB to LSB
  template<unsigned int ...bits>
  class Bit{
    static_assert(width == std::numeric_limits<T>::digits, 
        "Width must be width of register when using Bit");

    static constexpr auto size = sizeof...(bits);
    static constexpr auto bitArray = LL::array<unsigned int, size>{bits...};

    //Generates Mask for reading 
    template<unsigned int pos1, unsigned int... others>
    struct maskGen{
      static constexpr T mask = 1<<pos1 | maskGen<others...>::mask;
    };
    template<unsigned int pos>
    struct maskGen<pos>{
      static constexpr T mask = 1<<pos;
    };

    //generate the write value to write to multiple pins
    //inBit should be a List of the bits
    template<typename inBit, typename h, typename... t>
    AlwayInline static T getWriteValue(h first, t... values) {
      static_assert(std::numeric_limits<h>::is_integer, 
          "Write values must be integers");
      return (1&first)<<inBit::Value 
        | getWriteValue<typename inBit::Next>(values...);
    };
    template<typename inPins>
    AlwayInline static T getWriteValue(){
      return 0;
    }

    template<unsigned int N>
    AlwayInline static auto getBitSetValue(LL::BitSet<N> a){
      T value = 0;
      for (unsigned int i = 0; i < N; ++i){
        value |= (a[i])<<bitArray[(N-1)-i];
      }
      return value;
    }

    AlwayInline static T getReadValue(T value){
      T result = 0;
      for (unsigned int i = 0; i < size; ++i){
        result |= ((value>>bitArray[(size-1)-i])&1)<<i;
      }
      return result;
    }

    //A list of the bits
    using bitList = typename Meta::makeList<bits...>::Value;

  public:
    //Mask used to show what bits are being used in the register
    static constexpr T mask = maskGen<bits...>::mask;

    //write the same value to every bit
    AlwayInline static void writeAll(T value){
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

    //Reads the whole register tests the bit
    //if high write back the read to the register and return true
    //if low returns false
    AlwayInline static bool testAndSet(){
      static_assert(size == 1, "Only able to test 1 bit");
      auto value = Register::read();
      if((value>>bitArray[0])&1){
        Register::write(value);
        return true;
      }
      return false;
    }

    template<unsigned int N>
    AlwayInline static void write(BitSet<N> bitSet){
      static_assert(N == sizeof...(bits), "Unmatched size");
      if(mut_t == Access::wo){
        Bit::wwrite(bitSet);
      }else{
        Bit::rwrite(bitSet);
      }
    }

    template<unsigned int N>
    AlwayInline static void rwrite(BitSet<N> bitSet){
      static_assert(N == sizeof...(bits), "Unmatched size");
      T value = getBitSetValue(bitSet);
      T currentValue = Register::read();
      Register::wwrite((currentValue&~mask)|value);
    }

    template<unsigned int N>
    AlwayInline static void wwrite(BitSet<N> bitSet){
      static_assert(N == sizeof...(bits), "Unmatched size");
      T value = getBitSetValue(bitSet);
      Register::wwrite(value);
    }

    //write different values to the bits
    template<typename... H>
    AlwayInline static void write(H... values){
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
    AlwayInline static void rwrite(H... values){
      static_assert(sizeof...(values) == sizeof...(bits), 
          "You need the same number of write values and bits");
      static_assert(mut_t != Access::ro, 
          "Trying to write to a readonly register");
      T value = getWriteValue<bitList>(values...);
      T currentValue = Register::read();
      Register::wwrite((~mask & currentValue) | value);
    }

    //write different values to the bits overriding all bits
    template<typename... H>
    AlwayInline static void wwrite(H... values){
      static_assert(sizeof...(values) == sizeof...(bits), 
          "You need the same number of write values and bits");
      static_assert(mut_t != Access::ro, 
          "Trying to write to a readonly register");
      T value = getWriteValue<bitList>(values...);
      Register::wwrite(value);
    }
    
    AlwayInline static BitSet<size> read(){
      auto value = Register::read();
      value = getReadValue(value);     
      return BitSet<size>(value);
    }

  }; //End of class Bit<bits...>
}; //End of class Register<mux_t, addr, offset, width, T>

template<Device::Word addr, Device::Word... bits>
using RegBit = typename Register<Access::rw, addr>:: template Bit<bits...>;

}//End of namespace Register
