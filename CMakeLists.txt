cmake_minimum_required (VERSION 2.8)

set (CMAKE_TOOLCHAIN_FILE cmake/Arduino-MiniPro.cmake)

set (CMAKE_CROSSCOMPILING 1)

project (LEDBlink C CXX ASM)

unset (CMAKE_C_FLAGS)
unset (CMAKE_CXX_FLAGS)

set (CMAKE_C_FLAGS   "-mmcu=atmega328p -nostdlib")
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -x c++ -lgcc -std=c++14 -I${CMAKE_SOURCE_DIR}/include")

set (CMAKE_EXE_LINKER_FLAGS "-static -nostdlib")

set (SOURCE_DIR "source")
set (TTYUSB "/dev/tty.usbserial-A5025UZM")

add_executable ( LEDBlink
  ${SOURCE_DIR}/main.s
  ${SOURCE_DIR}/main.cpp
)

add_custom_command(
  TARGET LEDBlink POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ./LEDBlink -O ihex ${CMAKE_BINARY_DIR}/kernel.hex
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Convert the ELF output file to a binary image" 
)

add_custom_target( Upload 
  DEPENDS ${CMAKE_BINARY_DIR}/kernel.hex
  COMMAND avrdude -p m328p -c stk500v1 -b 57600 -P ${TTYUSB} -U flash:w:${CMAKE_BINARY_DIR}/kernel.hex
  COMMENT "Uploading to Arduino"
)

add_custom_command(
  TARGET LEDBlink POST_BUILD
  COMMAND ${CMAKE_OBJDUMP} -D LEDBlink > ${CMAKE_BINARY_DIR}/kernel.asm
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Disassebling" 
)
