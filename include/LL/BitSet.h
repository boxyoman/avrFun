#pragma once
#include "templateList.h"
#include <stdint.h>
#include "config.h"

namespace LL{

//Can only do up to 16 bits for now
template<unsigned int N>
class BitSet{
  //Replace with dynamic sizing
  using T = unsigned int;
  T bits;

public:
  const unsigned int Size = N;

  AlwayInline constexpr BitSet(unsigned long long val) :  bits((T)val) {}

  template<unsigned n>
  AlwayInline constexpr BitSet(BitSet<n>& a, unsigned offset = 0) : 
    BitSet(a.getValue()>>offset){}
  

  AlwayInline constexpr bool operator [](unsigned int i) const{
    return (bits>>i) & 1;
  }

  AlwayInline constexpr T getValue() const{
    return bits;
  }

  template<unsigned int n>
  AlwayInline const auto operator + (const BitSet<n>& a) const {
    return BitSet<N+n>(bits<<n | a.getValue());
  }
};//End of BitSet

}//End of LL
