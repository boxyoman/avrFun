#pragma once

namespace LowLevel{

/* Use to make sure data that should only be one bit is one bit */
class Bit {
bool data = 0;
public:
  template<typename a>
  Bit (a num) : data(1 & num){};

  template<typename t>
  t operator <<(t value){
    return data<<value;
  }

  template<typename t>
  void operator = (t value){
    data = 1 & value;
  }

  template<typename t>
  bool operator ==(t value){
    return data == value;
  }
};
}
