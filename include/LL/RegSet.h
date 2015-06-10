#pragma once
#include "Register.h"
#include "config.h"
#include "Array.h"

namespace LL{

template<std::size_t addr, bool shouldRead = true>
class RegSet {
  using T = Device::Word;
  static constexpr std::size_t size = std::numeric_limits<T>::digits;
  using reg = Reg<addr>;

  template<int bit, int... others>
  AlwayInline void writeToData(LL::BitSet<sizeof...(others)+1> value){
    Data[bit] = value[sizeof...(others)];
    writeToData<others...>(value);
  }
  template<int bit>
  AlwayInline void writeToData(LL::BitSet<1> value){
    Data[bit] = value[0];
  }

  static LL::BitSet<size> read(){
    if(shouldRead == true){
      return reg::read();
    }else{
      return 0;
    }
  }

  LL::BitSet<size> Data;
  bool wasFlushed = false;
public:
  AlwayInline RegSet () :
    Data( read() ){ }

  template<int... bits>
  AlwayInline void write(LL::BitSet<sizeof...(bits)> value){
    writeToData<bits...>(value);
  }

  AlwayInline void flush(){
    reg::wwrite(Data);
    wasFlushed = true;
  }
  
  AlwayInline ~RegSet (){
    if(!wasFlushed)
      reg::wwrite(Data.getValue());
  }

};

}
