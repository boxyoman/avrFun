#pragma once

#include "config.h"
#include <cstddef>

namespace LL{
template<class T, std::size_t N>
struct array{
  T _data[N];
  const T* data() const noexcept {
    return _data;
  }
  constexpr T const& operator [] (std::size_t i) const {
    return _data[i];
  }
  constexpr std::size_t size() const{
    return N;
  }
};
}
