#pragma once
#include "Register.h"
#include "PowerManager.h"
#include <string>

namespace Arduino{

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

  using UDRn   = LL::Register<LL::Access::wr, udrn, 0, 16, uint16_t>;
  using RXB = LL::Register<LL::Access::rw, udrn+1>;
  using TXB = LL::Register<LL::Access::rw, udrn>;

  using UCSRnA = LL::Register<LL::Access::wr, ucsrna>;
  using RXCn   = UCSRnA::template Bit<7>;
  using TXCn   = UCSRnA::template Bit<6>;
  using UDREn  = UCSRnA::template Bit<5>;
  using FEn    = UCSRnA::template Bit<4>;
  using DORn   = UCSRnA::template Bit<3>;
  using UPEn   = UCSRnA::template Bit<2>;
  using U2Xn   = UCSRnA::template Bit<1>;
  using MPCMn  = UCSRnA::template Bit<0>;

  using UCSRnB = LL::Register<LL::Access::wr, ucsrnb>;
  using RXENn  = UCSRnB::template Bit<4>;
  using TXENn  = UCSRnB::template Bit<3>;
  using UCSZn2 = UCSRnB::template Bit<2>;
  using RXB8n  = UCSRnB::template Bit<1>;
  using TXB8n  = UCSRnB::template Bit<0>;

  using UCSRnC = LL::Register<LL::Access::wr, ucsrnc>;
  using UMSELn = UCSRnC::template Bit<7,6>;
  using UPMn   = UCSRnC::template Bit<5,4>;
  using USBSn  = UCSRnC::template Bit<3>;
  using UCSZn  = UCSRnC::template Bit<2,1>;
  using UCPOLn = UCSRnC::template Bit<0>;

  using UBRR = LL::Register<LL::Access::wr, ubrrn, 0, 12, uint16_t>;

  static constexpr uint16_t baudValue(uint16_t BUAD){
    return Device::clk/(16*BUAD)-1;
  }
public:

  static void setup(uint16_t buad,
      LL::BitSet<3> ucsz = UCSZ::bit8){

    PowerManager::turnOnUSART();
    //set buad rate
    UBRR::write(baudValue(buad));
    
    //Enable tx and rx
    UCSRnB::template Bit<4,3>::write(1,1);
    //No parity
    UMSELn::write(0,0);

    //Enable 2x
    //U2Xn::write(1);

    //Set the size
    UCSZn::write(LL::BitSet<2>(ucsz));
    UCSZn2::write(LL::BitSet<1>(ucsz, 2));
  } 

  static void put(unsigned char c){
    while(!UDREn::read()[0]);
    UDRn::write(c);
  }

  static void write(const std::string& str){
    for(auto &c : str){
      put(c);
    }
  }
};

}