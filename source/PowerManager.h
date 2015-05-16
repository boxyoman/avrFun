#pragma once
#include "Register.h"

namespace Arduino {
class PowerManager {
  enum {
    smcrAddr = 0x53,
    prrAddr = 0x64,
  } addresses;
  using PRR = LL::Register<LL::Access::rw, prrAddr>;
  using SMCR = LL::Register<LL::Access::rw, smcrAddr>;
  using PRADC = PRR::template Bit<0>;
  using PRTIM0 = PRR::template Bit<5>;
  using PRTIM1 = PRR::template Bit<3>;
  using PRTIM2 = PRR::template Bit<7>;

public:
  static void turnOnTimer0(){
    PRTIM0::write(0);
  }
  static void turnOffTimer0(){
    PRTIM0::write(1);
  }

  static void turnOnTimer1(){
    PRTIM1::write(0);
  }
  static void turnOffTimer1(){
    PRTIM1::write(1);
  }

  static void turnOnTimer2(){
    PRTIM2::write(0);
  }
  static void turnOffTimer2(){
    PRTIM2::write(1);
  }
  static void turnOnAdc(){
    PRADC::write(0);
  }
  static void turnOffAdc(){
    PRADC::write(1);
  }
};

}//end of Arduino
