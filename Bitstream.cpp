#include "Bitstream.hpp"

namespace
{
    constexpr int BYTE_SIZE = 8;
}

Bitstream::Bitstream(const int capacity)
{
    bytes.reserve(capacity);
}

void Bitstream::get(int bitsCount, void* data)
{
    auto dataBytes = reinterpret_cast<std::byte*>(data);
    int bytesCount = 0;
    if ((firstByteIsFull()) && bitsCount >= BYTE_SIZE)
    {
        bytesCount = bitsCount / BYTE_SIZE;
        getBytes(bytesCount, dataBytes);
        bitsCount %= BYTE_SIZE;
    }
    else
    {
        while (bitsCount >= BYTE_SIZE)
        {
            bitsCount -= BYTE_SIZE;
            dataBytes[bytesCount++] = getBits(BYTE_SIZE);
        }
    }
    if (bitsCount > 0)
    {
        const auto mask = ~std::byte(0xFF << bitsCount);
        dataBytes[bytesCount] |= mask & getBits(bitsCount);
    }
}

void Bitstream::add(int bitsCount, void* data)
{
    auto dataBytes = reinterpret_cast<std::byte*>(data);
    int bytesCount = 0;
    if (lastByteIsFull() && bitsCount >= BYTE_SIZE)
    {
        if (bytes.empty())
        {
            bitsToReadOnFirstByte = 8;
        }
        bytesCount = bitsCount / BYTE_SIZE;
        addBytes(bytesCount, dataBytes);
        bitsCount %= BYTE_SIZE;
    }
    else
    {
        while (bitsCount >= BYTE_SIZE)
        {
            bitsCount -= BYTE_SIZE;
            addByte(dataBytes[bytesCount++]);
        }
    }
    if (bitsCount > 0)
    {
        addBits(bitsCount, dataBytes[bytesCount]);
    }
}

std::vector<std::byte> Bitstream::toBytes() const
{
    return bytes;
}

int Bitstream::count() const
{
    return bytes.size() * BYTE_SIZE;
}

void Bitstream::getBytes(const int bytesCount, std::byte* data)
{
    for (auto i = 0; i < bytesCount; ++i)
    {
        data[i] = bytes[i];
    }
    bytes.erase(std::begin(bytes), std::begin(bytes) + bytesCount);
}

std::byte Bitstream::getBits(const int bitsCount)
{
    auto result = std::byte();
    for (int i = 0; i < bitsCount; ++i)
    {
        result |= std::byte(getBit() << i);
    }
    return result;
}

bool Bitstream::getBit()
{
    auto& byte = bytes.front();
    auto result = bool(byte & std::byte(1));
    byte >>= 1;
    bitsToReadOnFirstByte--;
    if (bytes.size() == 1)
    {
        bitsWrittenOnLastByte--;
    }
    if (firstByteIsEmpty())
    {
        prepareNewByteToRead();
    }
    return result;
}

bool Bitstream::firstByteIsEmpty() const
{
    return bitsToReadOnFirstByte == 0;
}

bool Bitstream::firstByteIsFull() const
{
    return bitsToReadOnFirstByte == 8;
}

void Bitstream::prepareNewByteToRead()
{
    bytes.erase(std::begin(bytes));
    if (bytes.size() == 1)
    {
        bitsToReadOnFirstByte = bitsWrittenOnLastByte;
    }
    else
    {
        bitsToReadOnFirstByte = 8;
    }
}

void Bitstream::addBytes(const int bytesCount, std::byte* data)
{
    bytes.insert(bytes.end(), data, data + bytesCount);
}

void Bitstream::addByte(std::byte byte)
{
    addBits(8, byte);
}

void Bitstream::addBits(const int bitsCount, std::byte byte)
{
    for (int i = 0; i < bitsCount; ++i)
    {
        addBit(bool(byte & std::byte(0x1)));
        byte >>= 1;
    }
}

void Bitstream::addBit(const bool bit)
{
    if (lastByteIsFull())
    {
        prepareNewByteToWrite();
    }
    auto &byte = bytes.back();
    byte |= std::byte(bit << bitsWrittenOnLastByte);
    bitsWrittenOnLastByte++;
    if (bytes.size() == 1)
    {
        ++bitsToReadOnFirstByte;
    }
}

bool Bitstream::lastByteIsFull() const
{
    return bitsWrittenOnLastByte % 8 == 0;
}

void Bitstream::prepareNewByteToWrite()
{
    bytes.push_back(std::byte());
    bitsWrittenOnLastByte = 0;
}
