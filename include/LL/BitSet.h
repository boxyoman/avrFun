#pragma once
#include <stdint.h>
#include <cstddef>
#include <limits>
#include "config.h"
#include "templateList.h"

namespace LL{


//Can only do up to 16 bits for now
template<std::size_t N>
class BitSet{
  //TODO: Replace with dynamic sizing
  using T = std::size_t;
  static constexpr auto width = std::numeric_limits<T>::digits;

  static_assert(N <= width, 
      "BitSet: width greater than what is currently supported");

  AlwayInline static constexpr T generateMask(){
    return (N>=width)? ~0 : ((1 << N) - 1);
  }

  T bits;

public:
  const std::size_t Size = N;
  static constexpr T mask = generateMask();

  class Reference {
    T &data;
    std::size_t i;

  public:
    AlwayInline Reference (T &a, const std::size_t I) : data(a), i(I) {}

    AlwayInline Reference& operator= (bool value){
      data = (data & ~(0<<i)) | (value<<i);
      return *this;
    }

    AlwayInline operator bool(){
      return (data>>i)&1;
    }
  };

  AlwayInline constexpr BitSet() : bits(0) {}
  AlwayInline constexpr BitSet(unsigned long long val) :  bits(mask&val) {}

  template<std::size_t n>
  AlwayInline constexpr BitSet(const BitSet<n>& a, unsigned int offset = 0) : 
    BitSet(a.getValue()>>offset){}
  

  AlwayInline constexpr bool operator [](std::size_t i) const{
    return (bits>>i) & 1;
  }

  AlwayInline auto operator[](std::size_t i){
    return Reference(bits, i);
  }

  //Binary operators
  AlwayInline auto& operator &= (const BitSet<N>& other){
    bits &= other.getValue();
    return *this;
  }

  AlwayInline auto& operator |= (const BitSet<N>& other){
    bits |= other.getValue();
    return *this;
  }

  AlwayInline auto& operator ^= (const BitSet<N>& other){
    bits ^= other.getValue();
    return *this;
  }

  AlwayInline BitSet<N> operator ~(){
    return ~bits;
  }

  AlwayInline auto& operator <<=(std::size_t i){
    bits <<= i;
    return *this;
  }
  AlwayInline auto& operator >>=(std::size_t i){
    bits >>= i;
    return *this;
  }

  //template<std::size_t n>
  //AlwayInline auto& operator =(const BitSet<n>& a){
    //bits = (bits & ~a.mask) | a.bits;
    //return *this;
  //}
  
  AlwayInline BitSet<N> operator &(int a){
    return bits & a;
  }
  AlwayInline BitSet<N> operator |(int a){
    return bits | a;
  }

  AlwayInline BitSet<N> operator <<(std::size_t i) const{
    return bits<<i;
  }

  AlwayInline BitSet<N> operator >>(std::size_t i) const{
    return bits>>i;
  }


  AlwayInline constexpr T getValue() const{
    return bits&mask;
  }

  template<std::size_t n>
  AlwayInline const auto operator + (const BitSet<n>& a) const {
    return BitSet<N+n>(bits<<n | a.getValue());
  }

};//End of BitSet

// Some useful binary operators
template<std::size_t N, std::size_t n>
AlwayInline BitSet<N> operator &(const BitSet<N>& lhs, const BitSet<n>& rhs){
  return lhs.getValue() & rhs.getValue();
}

template<std::size_t N, std::size_t n>
AlwayInline BitSet<N> operator |(const BitSet<N>& lhs, const BitSet<n>& rhs){
  return lhs.getValue() | rhs.getValue();
}

template<std::size_t N, std::size_t n>
AlwayInline BitSet<N> operator ^(const BitSet<N>& lhs, const BitSet<n>& rhs){
  return lhs.getValue() ^ rhs.getValue();
}

}//End of LL
