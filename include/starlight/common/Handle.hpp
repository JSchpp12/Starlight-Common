#pragma once

#include <functional>
#include <cstddef>
#include <stdint.h>

namespace star
{
struct Handle
{
    bool operator==(const Handle &other) const
    {
        return id == other.id && type == other.type;
    }

    bool operator!() const noexcept
    {
        return !isInitialized();
    }

    bool isInitialized() const noexcept
    {
        return type != 0;
    }

    uint32_t &getID()
    {
        return id;
    }
    uint32_t getID() const
    {
        return id;
    }
    uint16_t &getType()
    {
        return type;
    }
    uint16_t getType() const
    {
        return type;
    }
    bool isSameElementAs(const Handle &other) const
    {
        return type == other.type && id == other.id;
    }

    uint16_t type = 0;
    uint32_t id = 0;
};

struct HandleHash
{
    std::size_t operator()(const Handle &handle) const noexcept
    {
        uint64_t key = (static_cast<uint64_t>(handle.getType()) << 32) | handle.getID();
        return std::hash<uint64_t>()(key);
    }
};

inline bool operator<(const Handle &lhs, const Handle &rhs)
{
    return lhs.getID() < rhs.getID();
}
} // namespace star
