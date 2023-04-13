#include "Bitstream2.hpp"

#include <algorithm>

namespace
{
    constexpr int BYTE_SIZE = 8;

    inline bool getBit(std::byte byte, const uint8_t bitOffset)
    {
        return bool(byte & std::byte(1 << bitOffset));
    }
}

Bitstream2::Bitstream2(int)
{
}

void Bitstream2::get(int bitsCount, void* data)
{
    auto dataBytes = reinterpret_cast<std::byte*>(data);
    const auto bytesCount = bitsCount / BYTE_SIZE;
    if (bytesCount > 0)
    {
        getBytes(bytesCount, dataBytes);
        bitsCount %= BYTE_SIZE;
    }
    if (bitsCount > 0)
    {
        const auto mask = ~std::byte(0xFF << bitsCount);
        dataBytes[bytesCount] |= mask & getBits(bitsCount);
    }
}

void Bitstream2::add(int bitsCount, void* data)
{
    auto dataBytes = reinterpret_cast<std::byte*>(data);
    const auto bytesCount = bitsCount / BYTE_SIZE;
    if (bytesCount > 0)
    {
        addBytes(bytesCount, dataBytes);
        bitsCount %= BYTE_SIZE;
    }
    if (bitsCount > 0)
    {
        addBits(bitsCount, dataBytes[bytesCount]);
    }
}

std::vector<std::byte> Bitstream2::toBytes() const
{
    std::vector<std::byte> bytes;
    const auto bytesCount = bits.size() / BYTE_SIZE;
    bytes.reserve(bytesCount);
    std::generate_n(std::back_inserter(bytes), bytesCount, [this, byteIdx = 0]() mutable
                    { return byte(byteIdx++); });
    return bytes;
}

void Bitstream2::getBytes(const int bytesCount, std::byte* data)
{
    for (auto byteIdx = 0; byteIdx < bytesCount; ++byteIdx)
    {
        data[byteIdx] = byte(byteIdx);
    }

    bits.erase(bits.begin(), bits.begin() + bytesCount * BYTE_SIZE);
}

std::byte Bitstream2::getBits(const int bitsCount)
{
    auto result = std::byte();
    for (int bitIdx = 0; bitIdx < bitsCount; ++bitIdx)
    {
        result |= std::byte(bits.at(bitIdx) << bitIdx);
    }
    bits.erase(bits.begin(), bits.begin() + bitsCount);
    return result;
}

std::byte Bitstream2::byte(const int byteIdx) const
{
    auto result = std::byte();
    for (int i = 0; i < BYTE_SIZE; ++i)
    {
        result |= std::byte(bits.at(byteIdx * BYTE_SIZE + i) << i);
    }
    return result;
}

void Bitstream2::addBytes(const int bytesCount, std::byte* data)
{
    for (auto byteIdx = 0; byteIdx < bytesCount; ++byteIdx)
    {
        addBits(8, data[byteIdx]);
    }
}

void Bitstream2::addBits(const int bitsCount, const std::byte byte)
{
    std::generate_n(std::back_inserter(bits), bitsCount, [byte, bitIdx = 0]() mutable
                    { return getBit(byte, bitIdx++); });
}
