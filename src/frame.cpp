#include "frame.h"

bool MacAddress::operator==(const MacAddress& other) const
{
    for (int i = 0; i < 6; ++i){
        if (bytes[i] != other.bytes[i]){
            return false;
        }
    }
    return true;
}

std::size_t MACHash::operator()(const MacAddress& address) const noexcept
{
    uint64_t value = 0;
    for (int i = 0; i < 6; ++i){
        value = value << 8 | static_cast<uint64_t>(address.bytes[i]);
    }
    return std::hash<uint64_t>{}(value);
}