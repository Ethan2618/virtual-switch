#pragma once

#include <cstdint>
#include <vector>
#include <array>

struct MacAddress
{
    std::array<uint8_t, 6> bytes;
    bool operator==(const MacAddress& other) const;
};

struct MACHash
{
    std::size_t operator()(const MacAddress& address) const noexcept;
};

struct EthernetFrame
{
    MacAddress source;
    MacAddress destination;
    std::vector<uint8_t> payload;
};