#pragma once

namespace Meta{

struct NIL{
  static const unsigned int Value = 0;
  using Next = NIL;
};

template<unsigned int value, class h>
struct List{
  static const unsigned int Value = value;
  using Next = h;
};

template<unsigned int h, unsigned int... t>
struct makeList{
  using Value = List<h, typename makeList<t...>::Value>;
};
template<unsigned int h>
struct makeList<h>{
  using Value = List<h, NIL>;
};

//Get the length of list
template<typename list, unsigned int n = 0>
struct Length{
  static constexpr auto Value = Length<typename list::Next, n+1>::Value;
};
template<unsigned int n>
struct Length<NIL, n>{
  static constexpr auto Value = n;
};

//Find the Nth element of list
template<typename list, unsigned int n>
struct Nth{
  static constexpr unsigned int Value = Nth<typename list::Next, n-1>::Value;
};
template<typename list>
struct Nth<list, 0>{
  static constexpr unsigned int Value = list::Value;
};

template<typename lista, typename listb>
struct isEqual{
  static constexpr unsigned int valueA = lista::Value;
  static constexpr unsigned int valueB = listb::Value;
  static constexpr bool Value = valueA==valueB && 
    isEqual<typename lista::Next, typename listb::Next>::Value;
};
template<>
struct isEqual<NIL, NIL>{
  static constexpr bool Value = true;
};

template<typename list, unsigned int n=Length<list>::Value>
struct reverseList{
  static constexpr auto nth = Nth<list, n>::Value;
  using Next = typename list::Next;
  using Value = List<nth, reverseList<Next, n-1>>;
};

}
