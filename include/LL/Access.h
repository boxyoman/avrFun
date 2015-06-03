#ifndef ACCESS_H
#define ACCESS_H

#include "BitSet.h"
#include "Device.h"
#include <limits>

namespace LL{
  namespace Access{
    //Structs in this namespace are meant to be used in conjuction with 
    //LL::Register

    template<typename T>
    using reg_t = volatile T*;

    template<typename T>
    constexpr auto size (){
      return std::numeric_limits<T>::digits;
    }

    //For C++14
    //template<typename T>
    //constexpr auto size std::numeric_limits<T>::digits; 
    
    //Register is read only
    struct ro{
      template<std::size_t addr, std::size_t offset, std::size_t width, 
        typename T = Device::Word>
      static BitSet<width> read(){
        reg_t<T> device = reinterpret_cast<reg_t<T>>(addr);
        return BitSet<width>(*device)>>offset;
      }
    };

    //Register is write only
    struct wo{
      template<std::size_t addr, std::size_t offset, std::size_t width, 
        typename T = Device::Word>
      static void wwrite(LL::BitSet<width> value){
        reg_t<T> device = reinterpret_cast<reg_t<T>>(addr);
        *device = (value<<offset).getValue();
      }
      template<std::size_t addr, std::size_t offset, std::size_t width, 
        typename T = Device::Word>
      static void write(LL::BitSet<width> value){
        wwrite<addr, offset, width, T>(value);
      }
    };

    // Register has read and write access
    struct wr: public ro, public wo{
      template<std::size_t addr, std::size_t offset, std::size_t width, 
        typename T = Device::Word>
      static void write(LL::BitSet<size<T>> value){
        reg_t<T> device = reinterpret_cast<reg_t<T>>(addr);

        auto readValue = read<addr, 0, size<T>(), T>();
        auto writeValue = (readValue & ~(value.mask<<offset)) | value<<offset;
        *device = writeValue.getValue();
      }

    private:
      template<typename T = Device::Word, 
        std::size_t s = size<T>()>
      auto getWriteValue(BitSet<s> readValue, BitSet<s> value, 
          std::size_t offset ){

        for (int i = 0; i < s; ++i){
          readValue[i+offset] = value[i];
        }
        return readValue;
      }
    };

    struct rw: public wr{};

  } //end of Access
} //end of LL

#endif /* end of include guard: ACCESS_H */
