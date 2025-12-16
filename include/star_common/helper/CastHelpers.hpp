#pragma once

#include <iostream>
#include <limits>
#include <stdint.h>

namespace star::common::helper
{
inline static uint32_t size_t_to_unsigned_int(size_t org)
{
    if (org > std::numeric_limits<uint32_t>::max())
    {
        std::cerr << "Error: Size too large for uint32_t" << std::endl;
        exit(1);
    }

    return static_cast<uint32_t>(org);
}

template <typename In, typename Out> bool SafeCast(const In &in, Out &out)
{
    static_assert(std::is_integral_v<In> && std::is_integral_v<Out>, "SafeCast only supports integral types.");

    if constexpr (std::is_signed_v<In> == std::is_signed_v<Out>)
    {
        // both signed OR both unsigned
        if (in >= std::numeric_limits<Out>::min() && in <= std::numeric_limits<Out>::max())
        {
            out = static_cast<Out>(in);
            return true;
        }
    }
    else if constexpr (std::is_unsigned_v<In> && std::is_signed_v<Out>)
    {
        // unsigned -> signed
        if (in <= static_cast<std::make_unsigned_t<Out>>(std::numeric_limits<Out>::max()))
        {
            out = static_cast<Out>(in);
            return true;
        }
    }
    else if constexpr (std::is_signed_v<In> && std::is_unsigned_v<Out>)
    {
        // signed -> unsigned
        if (in >= 0 && static_cast<std::make_unsigned_t<In>>(in) <= std::numeric_limits<Out>::max())
        {
            out = static_cast<Out>(in);
            return true;
        }
    }

    return false; // out of range
}
} // namespace star::CastHelpers