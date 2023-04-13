#pragma once
#include <cstddef>
#include <deque>
#include <vector>

class Bitstream2
{

public:
    Bitstream2(int initialCapacity);
    void get(int bytesCount, void* data);
    void add(int bytesCount, void* data);
    std::vector<std::byte> toBytes() const;

private:
    void getBytes(int bytesCount, std::byte* data);
    std::byte getBits(int bitsCount);
    std::byte byte(int byteIdx) const;
    void addBytes(int bytesCount, std::byte* data);
    void addBits(int bitsCount, std::byte byte);

private:
    std::deque<bool> bits;
};