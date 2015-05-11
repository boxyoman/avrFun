#pragma once
#include "Register.h"

namespace Arduino {
class PowerManager {
  enum {
    smcrAddr = 0x53,
    prrAddr = 0x64,
  } addresses;
  using PRR = LowLevel::Register<LowLevel::Access::rw, prrAddr>;
  using SMCR = LowLevel::Register<LowLevel::Access::rw, smcrAddr>;
  using PRADC = PRR::template Bit<0>;
  using PRTIM0 = PRR::template Bit<5>;

public:
  static void turnOnTimer0(){
    PRTIM0::write(0);
  }
  static void turnOffTimer0(){
    PRTIM0::write(1);
  }
  static void turnOnAdc(){
    PRADC::write(0);
  }
  static void turnOffAdc(){
    PRADC::write(1);
  }
};

}//end of Arduino
