#pragma once

#include "config.h"
#include <cstddef>
#include <stdio.h>

namespace LL{
template<class T>
struct Complex{
  T real;
  T imag;
  Complex(T real, T imag) : real(real), imag(imag) {}
  Complex() : real(0), imag(0) {}

  Complex<T> operator *(T rhs) {
    return Complex<T>(this->real * rhs, this->imag * rhs);
  }
  
  Complex<T> operator +(Complex<T> rhs) {
    return Complex<T>(this->real + rhs.real, this->imag + rhs.imag);
  }

  Complex<T> operator +=(Complex<T> rhs) {
    this->real += rhs.real;
    this->imag += rhs.imag;
    return *this;
  }

};
}


