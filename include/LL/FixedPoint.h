#pragma once

#include "config.h"
#include <cstddef>
#include <stdint.h>

namespace LL{

template<typename type, std::size_t factor>
struct Fixed {
  //The value of the FixedPoint
  type value;

  static constexpr auto bitnum = factor;
  static constexpr float pow = uint64_t(1)<<bitnum;

  //Convert from floating point to fixed point
  Fixed (float num) : value(num*pow) { }
  Fixed () : value(0) { }

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
  Fixed<type, factor> operator += (Fixed<rhsType, factor> rhs){
    value = value + rhs.value;
    return *this;
  }

  //multiple two fixed point numbers
  template<typename rhsType>
  Fixed<rhsType, factor*2> mult(Fixed<type, factor> rhs){
    using rType = Fixed<rhsType, factor*2>;
    auto result = rType(0);
    result.value = static_cast<typename rType::type>(value) * 
      static_cast<typename rType::type>(rhs.value);
    return result;
  }

  //multiple two fixed point numbers
  Fixed<type, factor*2> operator * (Fixed<type, factor> rhs){
    using rType = Fixed<type, factor*2>;
    auto result = rType(0);
    result.value = static_cast<typename rType::type>(value) * 
      static_cast<typename rType::type>(rhs.value);
    return result;
  }

}; //End of Fix

}//end of LL
