#pragma once
#include "templateList.h"
#include <stdint.h>

namespace LowLevel{

//How I want to implement it
//I'm not good enough :( 
//Why can't there be a STL for AVR
class BitSet{
  bool bit0;
  bool bit1;
  bool bit2;
public:
  constexpr BitSet(bool b0, bool b1, bool b2): 
    bit0(b0), 
    bit1(b1),
    bit2(b2){ }

  constexpr bool operator [] (int i) const{
    return (i==0) ? bit0 : (i==1) ? bit1 : bit2;
  }
  constexpr bool operator != (BitSet a){
    return a[0] != bit0 || a[1] != bit1 || a[2] != bit2;
  }
  constexpr bool operator == (BitSet a){
    return a[0] == bit0 && a[1] == bit1 && a[2] == bit2;
  }
};

}
