#include "Bitstream.hpp"
#include <cstring>
#include <cassert>

template<typename BitstreamType>
class BitstreamTests
{
public:

static void runAll() {
  testWord();
  testByte();
  testOneBit();
  testTwoBits();
  test4Bits();
  testByteWritten2BitsRead();
  testByteWritten4BitsRead();
  testLessThanByteSize();
}


static void testWord() {
  std::byte data[4] = {std::byte(0x00), std::byte(0x11), std::byte(0x22), std::byte(0x33)};
  std::byte output[4] = {std::byte(), std::byte(), std::byte(), std::byte()};
  
  BitstreamType bs(600);
  bs.add(32, data);
  bs.get(32, output);
  
  assert(std::memcmp(data, output, sizeof(data)) == 0);
}


static void testByte() {
  std::byte data[4] = {std::byte(0x33), std::byte(0x11), std::byte(0x22), std::byte(0x44)};
  std::byte output[1] = {std::byte()};
  
  BitstreamType bs(600);
  bs.add(32, data);
  bs.get(8, output);
  
  assert(data[0] == output[0]);
}

static void testOneBit() {
  std::byte data[1] = {std::byte(0b1)};
  std::byte output[1] = {std::byte()};
  
  BitstreamType bs(600);
  bs.add(1, data);
  bs.get(1, output);
  assert(data[0] == output[0]);
}

static void testTwoBits() {
  std::byte data[1] = {std::byte(0b10)};
  std::byte output[1] = {std::byte()};
  
  BitstreamType bs(600);
  bs.add(2, data);
  bs.get(2, output);
  assert(data[0] == output[0]);
}

static void test4Bits() {
  std::byte data[1] = {std::byte(0b1001)};
  std::byte output[1] = {std::byte()};
  
  BitstreamType bs(600);
  bs.add(4, data);
  bs.get(4, output);
  assert(data[0] == output[0]);
}

static void testByteWritten2BitsRead() {
  std::byte data[1] = {std::byte(0b0111001)};
  std::byte output[1] = {std::byte()};
  
  BitstreamType bs(600);
  bs.add(8, data);
  bs.get(2, output);
  assert((data[0] & std::byte(0b11)) == output[0]);
}

static void testByteWritten4BitsRead() {
  std::byte data[1] = {std::byte(0b0111001)};
  std::byte output[1] = {std::byte()};
  
  BitstreamType bs(600);
  bs.add(8, data);
  bs.get(4, output);
  assert((data[0] & std::byte(0x0F)) == output[0]);
}

static void testLessThanByteSize() {
  std::byte data[4] = {std::byte(0x33), std::byte(0x11), std::byte(0x22), std::byte(0x44)};
  std::byte output[1] = {std::byte()};
  
  BitstreamType bs(600);
  bs.add(32, data);
  bs.get(5, output);
  assert((data[0] & std::byte(0x1F)) == output[0]);
}

};