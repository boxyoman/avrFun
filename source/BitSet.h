#pragma once
#include "templateList.h"
#include <stdint.h>
#include "config.h"

namespace LL{

template<unsigned int N>
class BitSet{
protected:
  BitSet<N-1> _next;
  using T = bool;
  T _bit;

public:
  const unsigned int Size = N;

  template<typename... T>
  AlwayInline constexpr BitSet(unsigned long long val) : 
    _next(BitSet<N-1>(val)),
    _bit((val>>(N-1))&1){}

  template<unsigned int n>
  AlwayInline constexpr BitSet(const BitSet<n>& a, unsigned int offset = 0) :
    BitSet(a.getValue()>>offset) {}

  AlwayInline constexpr T operator [](unsigned int i) const{
    return (i==Size-1) ? 1&_bit : _next[i];
  }

  AlwayInline constexpr unsigned long long getValue() const{
    return _bit<<(N-1) | _next.getValue();   
  }

  template<unsigned int n>
  AlwayInline constexpr BitSet<N+n> operator + (const BitSet<n>& a) const{
    return BitSet<N+n>(getValue()<<n | a.getValue());
  }

  AlwayInline constexpr T operator != (const BitSet<N>& a) const{
    return a[N-1] != _bit || a._next != _next;
  }
};

template<>
class BitSet<0>{
protected:
  using T = bool;
public:
  AlwayInline constexpr BitSet(unsigned long long val) {}
  AlwayInline constexpr T operator [](unsigned int i) const{
    return 0;
  }
  AlwayInline constexpr T operator !=(const BitSet<1>& a) const{
    return false;
  }
  AlwayInline constexpr unsigned long long getValue() const{
    return 0;   
  }
};

}//End of LL
