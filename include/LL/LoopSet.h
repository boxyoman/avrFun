#pragma once

#include "config.h"
#include <cstddef>

namespace LL{
template<class T, std::size_t N>
struct LoopSet{
private:
  T _data[N];
  std::size_t nextIndex = N-1;
  
public:
  void push(T value){
    _data[nextIndex] = value;
    if(nextIndex == 0){
      nextIndex = N-1;
    }else{
      nextIndex--;
    }
  }

  T& operator [] (std::size_t i) {
    auto index = nextIndex+1+i;
    if(index >= N){
      index = index-N;
    }
    return _data[index];
  }

  std::size_t size() const{
    return N;
  }
};
}

