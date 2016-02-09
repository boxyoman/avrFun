#pragma once
#include "LL/Register.h"
#include "PowerManager.h"
#include <limits>

namespace Arduino{

constexpr int power(int base, int exp){
  return ((exp == 0) ? 1 : base * power(base, --exp));
}

namespace UCSZ{
  constexpr auto bit5 = LL::BitSet<3>(0);
  constexpr auto bit6 = LL::BitSet<3>(1);
  constexpr auto bit7 = LL::BitSet<3>(2);
  constexpr auto bit8 = LL::BitSet<3>(3);
  constexpr auto bit9 = LL::BitSet<3>(7);
}

class USART {
  enum {
    udrn   = 0xc6,
    ucsrna = 0xc0,
    ucsrnb,
    ucsrnc,
    ubrrn  = 0xc4,
  };//Addresses

  using UDRn   = LL::Reg<udrn, 0, 16>;

  using UCSRnA = LL::Reg<ucsrna>;
  //using RXCn   = UCSRnA::template Bit<7>;
  //using TXCn   = UCSRnA::template Bit<6>;
  //using UDREn  = UCSRnA::template Bit<5>;
  //using FEn    = UCSRnA::template Bit<4>;
  //using DORn   = UCSRnA::template Bit<3>;
  //using UPEn   = UCSRnA::template Bit<2>;
  //using U2Xn   = UCSRnA::template Bit<1>;
  //using MPCMn  = UCSRnA::template Bit<0>;

  using UCSRnB = LL::Reg<ucsrnb>;
  //using RXENn  = UCSRnB::template Bit<4>;
  //using TXENn  = UCSRnB::template Bit<3>;
  //using UCSZn2 = UCSRnB::template Bit<2>;
  //using RXB8n  = UCSRnB::template Bit<1>;
  //using TXB8n  = UCSRnB::template Bit<0>;

  using UCSRnC = LL::Reg<ucsrnc>;
  //using UMSELn = UCSRnC::template Bit<7,6>;
  //using UPMn   = UCSRnC::template Bit<5,4>;
  //using USBSn  = UCSRnC::template Bit<3>;
  //using UCSZn  = UCSRnC::template Bit<2,1>;
  //using UCPOLn = UCSRnC::template Bit<0>;

  using UBRR = LL::Reg<ubrrn, 0, 12>;

  static constexpr uint16_t baudValue(uint16_t BUAD){
    return Device::clk/16/BUAD-1;
  }
public:

  AlwayInline static void setup(uint16_t buad,
      LL::BitSet<3> ucsz = UCSZ::bit8){

    PowerManager::turnOnUSART();
    //set buad rate
    UBRR::write(baudValue(buad));
    
    //Enable tx and rx
    UCSRnB::write<4,3>(0b11);
    //No parity
    UCSRnC::write<7,6>(0);

    //Set the size
    UCSRnC::write<2,1>(ucsz);
    UCSRnB::write<2>(ucsz>>2);
  } 

  NeverInline static void write(const char* str){
    do{
      put(*str);
    }while(*(++str) != '\0');
  }

  template<typename T>
  NeverInline static void writeNum(T number){
    static_assert(std::numeric_limits<T>::is_integer, 
        "type is not an integer");

    T highNum = power(10, std::numeric_limits<T>::digits10);
    do{
      put((number/highNum) | 0x30);
      number = number % highNum;
    }while((highNum /= 10));
  }

  static void put(unsigned char c){
    while(!UCSRnA::read()[5]);
    UDRn::write(c);
  }
};

}
