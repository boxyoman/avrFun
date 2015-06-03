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
  typename mut_t = Access::rw, 
  std::size_t offset = 0, 
  std::size_t width = std::numeric_limits<Device::Word>::digits, 
  typename T = Device::Word
  >
class reg {
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

  AlwayInline static auto read(){
    return mut_t::read();
  }

  AlwayInline static void write(LL::BitSet<width> val){
    mut_t::write(val);
  }

  //Called when writing has only write access
  AlwayInline static void wwrite(LL::BitSet<width> val){
    mut_t::wwrite(val);
  }

  // A class for easy simultaneous bit manipulations
  // Order from MSB to LSB
  //template<unsigned int ...bits>
  //class Bit{
    //static_assert(width == std::numeric_limits<T>::digits, 
        //"Width must be width of register when using Bit");

    //static constexpr auto size = sizeof...(bits);
    //static constexpr auto bitArray = LL::array<unsigned int, size>{{bits...}};

    ////Generates Mask for reading 
    //template<unsigned int pos1, unsigned int... others>
    //struct maskGen{
      //static constexpr T mask = 1<<pos1 | maskGen<others...>::mask;
    //};
    //template<unsigned int pos>
    //struct maskGen<pos>{
      //static constexpr T mask = 1<<pos;
    //};

    ////generate the write value to write to multiple pins
    ////inBit should be a List of the bits
    //template<typename inBit, typename h, typename... t>
    //AlwayInline static const T getWriteValue(const h first, t... values) {
      //static_assert(std::numeric_limits<h>::is_integer, 
          //"Write values must be integers");
      //return (1&first)<<inBit::Value 
        //| getWriteValue<typename inBit::Next>(values...);
    //};
    //template<typename inBit>
    //AlwayInline static T getWriteValue(){
      //return 0;
    //}

    //template<unsigned int N>
    //AlwayInline static const auto getBitSetValue(const LL::BitSet<N> a){
      //T value = 0;
      //for (unsigned int i = 0; i < N; ++i){
        //value |= (a[i])<<bitArray[(N-1)-i];
      //}
      //return value;
    //}

    //AlwayInline static T getReadValue(const T value){
      //T result = 0;
      //for (unsigned int i = 0; i < size; ++i){
        ////For the future after you move eveerything over to BitSet
        ////auto bit = bitArray[(size-1)-i];
        //result |= ((value>>bitArray[(size-1)-i])&1)<<i;
      //}
      //return result;
    //}

    ////A list of the bits
    //using bitList = typename Meta::makeList<bits...>::Value;

  //public:
    ////Mask used to show what bits are being used in the register
    //static constexpr T mask = maskGen<bits...>::mask;

    ////write the same value to every bit
    //AlwayInline static void writeAll(const T value){
      //static_assert(mut_t != Access::ro, 
          //"Trying to write to a readonly register");
      //value &= 1;
      //T currentValue = (mut_t != Access::ro) ? Register::read() : 0;
      //if(value == 1){
        //Register::wwrite(mask | currentValue);
      //}else{
        //Register::wwrite(~mask & currentValue);
      //}
    //}

    ////Reads the whole register tests the bit
    ////if high write back the read to the register and return true
    ////if low returns false
    //AlwayInline static bool testAndSet(){
      //static_assert(size == 1, "Only able to test 1 bit");
      //auto value = Register::read();
      //if((value>>bitArray[0])&1){
        //Register::write(value);
        //return true;
      //}
      //return false;
    //}

    //AlwayInline static void write(const BitSet<size> bitSet){
      //if(mut_t == Access::wo){
        //Bit::wwrite(bitSet);
      //}else{
        //Bit::rwrite(bitSet);
      //}
    //}

    //AlwayInline static void rwrite(const BitSet<size> bitSet){
      //T value = getBitSetValue(bitSet);
      //T currentValue = Register::read();
      //Register::wwrite((currentValue&~mask)|value);
    //}

    //AlwayInline static void wwrite(const BitSet<size> bitSet){
      //T value = getBitSetValue(bitSet);
      //Register::wwrite(value);
    //}
    
    //AlwayInline static BitSet<size> read(){
      //auto value = Register::read();
      //value = getReadValue(value);     
      //return BitSet<size>(value);
    //}

  //}; //End of class Bit<bits...>
}; //End of class Register<mux_t, addr, offset, width, T>

}//End of namespace Register
