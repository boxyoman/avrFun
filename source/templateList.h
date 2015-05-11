#pragma once

namespace Meta{

struct NIL{
  static const int Value = 0;
  using Next = NIL;
};

template<int value, class h>
struct List{
  static const int Value = value;
  using Next = h;
};

template<int h, int... t>
struct makeList{
  using Value = List<h, typename makeList<t...>::Value>;
};

template<int h>
struct makeList<h>{
  using Value = List<h, NIL>;
};

}
