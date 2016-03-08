#pragma once
#include <stdint.h>
#include <cstddef>

namespace LL{

namespace{

constexpr std::size_t getByteCount(std::size_t bitCount){
  return bitCount/8 + ((bitCount%8>0)? 1 : 0);
}

template<std::size_t bitCount, bool isSigned> struct bitType_;
template<> struct bitType_<1, 0> { using type = uint8_t; };
template<> struct bitType_<2, 0> { using type = uint16_t; };
template<> struct bitType_<3, 0> { using type = uint32_t; };
template<> struct bitType_<4, 0> { using type = uint32_t; };
template<> struct bitType_<5, 0> { using type = uint64_t; };
template<> struct bitType_<6, 0> { using type = uint64_t; };
template<> struct bitType_<7, 0> { using type = uint64_t; };
template<> struct bitType_<8, 0> { using type = uint64_t; };
//Signed
template<> struct bitType_<1, 1> { using type = int8_t; };
template<> struct bitType_<2, 1> { using type = int16_t; };
template<> struct bitType_<3, 1> { using type = int32_t; };
template<> struct bitType_<4, 1> { using type = int32_t; };
template<> struct bitType_<5, 1> { using type = int64_t; };
template<> struct bitType_<6, 1> { using type = int64_t; };
template<> struct bitType_<7, 1> { using type = int64_t; };
template<> struct bitType_<8, 1> { using type = int64_t; };

} //end of Anonymous Namespace

template<std::size_t bitCount, bool isSigned=0> 
using bitType = typename bitType_<getByteCount(bitCount), isSigned>::type;

}//end of LL
