#pragma once
#include "config.h"
#include "BitSet.h"

namespace Arduino{

//OCxn modes of operation
//page 105
namespace OC{
  constexpr auto Normal = LL::BitSet<2>(0);
  constexpr auto Toggle = LL::BitSet<2>(1);
  constexpr auto Set    = LL::BitSet<2>(2);
  constexpr auto Clear  = LL::BitSet<2>(3);
}

//Wave Form Generation Modes
//page 107
namespace WGM{
  constexpr auto Normal   = LL::BitSet<3>(0);
  constexpr auto Phase    = LL::BitSet<3>(1);
  constexpr auto CTC      = LL::BitSet<3>(2); //Clear time on compare
  constexpr auto Fast     = LL::BitSet<3>(3);
  constexpr auto PhaseToA = LL::BitSet<3>(0b101);
  constexpr auto FastToA  = LL::BitSet<3>(0b111);
};

//Clock Select
namespace CS{
  constexpr auto NoClk   = LL::BitSet<3>(0);
  constexpr auto Clk     = LL::BitSet<3>(1);
  constexpr auto Clk8    = LL::BitSet<3>(2);
  constexpr auto Clk64   = LL::BitSet<3>(3);
  constexpr auto Clk256  = LL::BitSet<3>(4);
  constexpr auto Clk1024 = LL::BitSet<3>(5);
  constexpr auto ExtRise = LL::BitSet<3>(6);
  constexpr auto ExtFall = LL::BitSet<3>(7);
};

} //End of Arduino

