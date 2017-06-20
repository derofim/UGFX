// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include <cstdint> 

namespace exa {

// Types existing on all platforms
struct genericTypes
{
    using uint8  = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using int8   = int8_t;
    using int16  = int16_t;
    using int32  = int32_t;
    using int64  = int64_t;
};

using uint8 = genericTypes::uint8;
using uint16 = genericTypes::uint16;
using uint32 = genericTypes::uint32;
using uint64 = genericTypes::uint64;

using int8 = genericTypes::int8;
using int16 = genericTypes::int16;
using int32 = genericTypes::int32;
using int64 = genericTypes::int64;

} // namespace exa