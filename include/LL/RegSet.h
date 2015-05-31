#pragma once
#include "Register.h"
#include "config.h"
#include "Array.h"

namespace LL{

template<std::size_t addr>
class RegSet {
  using T = Device::Word;
  static constexpr std::size_t size = std::numeric_limits<T>::digits;

  template<int bit, int... others>
  AlwayInline void writeToData(LL::BitSet<sizeof...(others)+1> value){
    Data = (Data & ~(1<<bit)) | value[sizeof...(others)]<<bit;
    writeToData<others...>(value);
  }
  template<int bit>
  AlwayInline void writeToData(LL::BitSet<1> value){
    Data = (Data & ~(1<<bit)) | value[0]<<bit;
  }

  T Data;
  bool wasFlushed = false;
  using reg = Register<Access::wr, addr>;
public:
  AlwayInline RegSet (){
    Data = reg::read();
  }

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
      reg::wwrite(Data);
  }

};

}
