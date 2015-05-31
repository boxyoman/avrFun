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

  class Reference {
  public:
    AlwayInline Reference (T &a, const std::size_t I) : data(a), i(I) {}

    AlwayInline Reference& operator= (bool value){
      data = (data & ~(0<<i)) | (value<<i);
      return *this;
    }

    AlwayInline operator bool(){
      return (data>>i)&1;
    }
  
  private:
    T &data;
    std::size_t i;
    /* data */
  };

  AlwayInline constexpr BitSet(unsigned long long val) :  bits((T)val) {}

  template<std::size_t n>
  AlwayInline constexpr BitSet(const BitSet<n>& a, unsigned int offset = 0) : 
    BitSet(a.getValue()>>offset){}
  

  AlwayInline constexpr bool operator [](std::size_t i) const{
    return (bits>>i) & 1;
  }

  AlwayInline auto operator[](std::size_t i){
    return Reference(bits, i);
  }

  AlwayInline constexpr BitSet<N> operator >>(std::size_t i) const{
    return bits>>i;
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
