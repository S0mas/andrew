#include <iostream>
#include "Bitstream.hpp"
#include "Bitstream2.hpp"
#include "BitstreamTests.hpp"
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>

inline std::ostream& operator<< (std::ostream& os, std::byte b) {
	return os << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<int>(b);
}

struct object
{
    std::byte Var0; // : 4;
    std::byte Var1; // : 2;
    std::byte Var2; // : 5;
    std::byte Var3; // : 1;
    std::byte Var4;
    uint16_t Var5;
    std::byte Var6; // : 4;
};

void print(const std::vector<std::byte>& bytes)
{
    int k = 0;
    for (int i = 0; i < bytes.size();)
    {
        std::cout << std::dec << std::setw(3) << std::setfill(' ') << k++ << ". ";
        for (int j = 0; i < bytes.size() && j < 2; ++i, ++j)
        {
            std::cout << bytes.at(i);
        }
        std::cout << " ";
        for (int j = 0; i < bytes.size() && j < 2; ++i, ++j)
        {
            std::cout << bytes.at(i);
        }
        std::cout << std::endl;
    }
}

void measureTime(std::function<void()> func, int count = 1)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < count; ++i)
        func();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "Time: " << duration << " microseconds" << std::endl;
}


template<typename T>
void testAddByte(T& bitstream) {
    std::byte dataIn = std::byte(0x11);
    bitstream.add(8, (void*)&dataIn);
}

template<typename T>
void testGetByte(T& bitstream) {
    std::byte dataOut;
    bitstream.get(8, (void*)&dataOut);
}

int main(int, char**) {
    Bitstream bitstream(8000);
    Bitstream2 bitstream2(64000);

    measureTime([&bitstream](){ testAddByte(bitstream); }, 8000);
    measureTime([&bitstream2](){ testAddByte(bitstream2); }, 8000);

    measureTime([&bitstream](){ testGetByte(bitstream); }, 8000);
    measureTime([&bitstream2](){ testGetByte(bitstream2); }, 8000);
    //measureTime([](){ BitstreamTests<Bitstream>::runAll(); }, 1000);
    //measureTime([](){ BitstreamTests<Bitstream2>::runAll(); }, 1000);
    BitstreamTests<Bitstream>::runAll();
    BitstreamTests<Bitstream2>::runAll();
    return 0;
}
