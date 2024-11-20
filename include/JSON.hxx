#pragma once

#ifndef JSON_HXX
#define JSON_HXX

#include <cstdint>

// Haven't completed this class yet
class JSON
{
public:
    using EnumBaseType = std::uint64_t;
    enum Type : EnumBaseType
    {
        object  = 0x01,
        array   = 0x02,
        string  = 0x04,
        number  = 0x08,
        boolean = 0x10,
        null    = 0x0,
    };
protected:
    Type type;
};

#endif // JSON_HXX
