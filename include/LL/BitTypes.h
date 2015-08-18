#pragma once
#include <stdint.h>
#include <cstddef>

namespace LL{

namespace{

constexpr std::size_t getByteCount(std::size_t bitCount){
  return bitCount/8 + ((bitCount%8>0)? 1 : 0);
}

template<std::size_t bitCount> struct bitType_;
template<> struct bitType_<1> { using type = uint8_t; };
template<> struct bitType_<2> { using type = uint16_t; };
template<> struct bitType_<3> { using type = uint32_t; };
template<> struct bitType_<4> { using type = uint32_t; };
template<> struct bitType_<5> { using type = uint64_t; };
template<> struct bitType_<6> { using type = uint64_t; };
template<> struct bitType_<7> { using type = uint64_t; };
template<> struct bitType_<8> { using type = uint64_t; };

} //end of Anonymous Namespace

template<std::size_t bitCount> 
using bitType = typename bitType_<getByteCount(bitCount)>::type;

}//end of LL
