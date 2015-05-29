#pragma once
#include <stdint.h>
#include <cstddef>
#include "config.h"
#include "templateList.h"

namespace LL{


//Can only do up to 16 bits for now
template<std::size_t N>
class BitSet{
  //TODO: Replace with dynamic sizing
  using T = std::size_t;
  T bits;

public:
  const std::size_t Size = N;

  AlwayInline constexpr BitSet(unsigned long long val) :  bits((T)val) {}

  template<std::size_t n>
  AlwayInline constexpr BitSet(const BitSet<n>& a, unsigned int offset = 0) : 
    BitSet(a.getValue()>>offset){}
  

  AlwayInline constexpr bool operator [](std::size_t i) const{
    return (bits>>i) & 1;
  }

  AlwayInline constexpr T getValue() const{
    return bits;
  }

  template<std::size_t n>
  AlwayInline const auto operator + (const BitSet<n>& a) const {
    return BitSet<N+n>(bits<<n | a.getValue());
  }
};//End of BitSet

}//End of LL
