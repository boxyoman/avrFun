/*
 *  A fixed point library. Depends on BitTypes
 *
 *  Doesn't work for signed ints yet.
 *
 *  Fixed<intType, factor>(float)
 *    Initialize the FixedPoint with the value of float
 *
 *  Fixed<intType, factor>(int)
 *    Initialize Fixed.value to the value of int
 *
 *  Example:
 *    auto number = Fixed<uint32_t, 4>(2.0f);
 *    auto other = Fixed<uint32_t, 4>(3.0f);
 */
#pragma once

#include "config.h"
#include <cstddef>
#include <stdint.h>
#include <limits>
#include "LL/BitTypes.h"

namespace LL{

//Doesn't do signed integers
//Need to update it later to take care of it.
template<typename T, std::size_t factor>
struct Fixed {
  //The value of the FixedPoint
  static constexpr bool isSigned = std::numeric_limits<T>::is_signed;
  static constexpr auto bitSize = std::numeric_limits<T>::digits;
   
  using type = T;
  type value;

  static constexpr auto bitnum = factor;
  static constexpr float pow = uint64_t(1)<<bitnum;

  //Convert from floating point to fixed point
  constexpr Fixed (float num) : value(num*pow) { }
  constexpr Fixed () : value(0) { }

  //template<std::size_t s, std::size_t f>
  //explicit Fixed (Fixed<s,f> num) : 
    //value(num.value << (factor-f)) { }

  //Convert from fixed point to float
  AlwayInline float toFloat(){
    return value/pow;
  }

  //Allow assigning of value from float
  AlwayInline void operator = (float num){
    value = num*pow;
  }

  //add two fixed point numbers
  template<typename rhsType>
  Fixed<rhsType, factor> operator + (Fixed<rhsType, factor> rhs){
    auto result = Fixed<rhsType, factor>(0);
    result.value = value + rhs.value;
    return result;
  }
  
  template<typename rhsType>
  Fixed<rhsType, factor> operator - (Fixed<rhsType, factor> rhs){
    auto result = Fixed<rhsType, factor>(0);
    result.value = value - rhs.value;
    return result;
  }

  template<typename rhsType>
  Fixed<type, factor> operator += (Fixed<rhsType, factor> rhs){
    value = value + rhs.value;
    return *this;
  }

  //multiple two fixed point numbers
  template<typename rhsType>
  auto mult(Fixed<rhsType, factor> rhs){
    //Set up the return type
    using returnIntType = LL::bitType<bitSize*2, isSigned>;
    using rType = Fixed<returnIntType, factor*2>;

    auto result = rType(0);
    result.value = static_cast<typename rType::type>(value) * 
      static_cast<typename rType::type>(rhs.value);
    return result;
  }

  //multiple two fixed point numbers
  Fixed<type, factor> operator * (Fixed<type, factor> rhs){
    //Convert Fixed<type*2, factor*2> to Fixed<type, factor>
    auto multipleResult = mult(rhs);
    auto result = Fixed<type, factor>();
    result.value = multipleResult.value>>factor;
    return result;
  }

}; //End of Fixed

}//end of LL
