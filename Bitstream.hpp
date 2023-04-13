#pragma once
#include <cstddef>
#include <vector>

class Bitstream
{

public:
    Bitstream(int capacity);

    void get(int bitLength, void *data);
    void add(int bitLength, void *data);
    std::vector<std::byte> toBytes() const;
    int count() const;

private:
    void getBytes(int bytesCount, std::byte* data);
    std::byte getBits(int bitsCount);
    bool getBit();
    bool firstByteIsEmpty() const;
    bool firstByteIsFull() const;
    void prepareNewByteToRead();

private:
    void addBytes(int bytesCount, std::byte* data);
    void addByte(std::byte byte);
    void addBits(int bitsCount, std::byte byte);
    void addBit(bool bit);
    bool lastByteIsFull() const;
    void prepareNewByteToWrite();

private:
    std::vector<std::byte> bytes;
    std::size_t bitsWrittenOnLastByte = 0;
    std::size_t bitsToReadOnFirstByte = 0;
};
