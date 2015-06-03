#pragma once
#include "LL/Register.h"

namespace Arduino {
class PowerManager {
  enum {
    smcrAddr = 0x53,
    prrAddr  = 0x64,
  } addresses;

  using SMCR = LL::Reg<smcrAddr, LL::Access::rw>;
  using PRR  = LL::Reg<prrAddr,  LL::Access::rw>;

public:
  static void turnOnUSART(){
    PRR::write<1>(0);
  }
  static void turnOffUSART(){
    PRR::write<1>(1);
  }

  static void turnOnTimer0(){
    PRR::write<5>(0);
  }
  static void turnOffTimer0(){
    PRR::write<5>(1);
  }

  static void turnOnTimer1(){
    PRR::write<3>(0);
  }
  static void turnOffTimer1(){
    PRR::write<3>(1);
  }

  static void turnOnTimer2(){
    PRR::write<6>(0);
  }
  static void turnOffTimer2(){
    PRR::write<6>(1);
  }
  static void turnOnAdc(){
    PRR::write<0>(0);
  }
  static void turnOffAdc(){
    PRR::write<0>(1);
  }
};

}//end of Arduino
