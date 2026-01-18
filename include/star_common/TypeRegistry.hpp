#pragma once

#include <absl/container/flat_hash_map.h>
#include <cassert>
#include <cstdint>
#include <mutex>
#include <optional>
#include <string_view>

namespace star::common
{
class TypeRegistry
{
  public:
    // Returns a unique dynamic type ID
    uint16_t registerType(std::string_view name)
    {
        std::lock_guard lock(m_mutex);

        if (!doContains(name))
        {
            uint16_t type = m_nextType++;
            m_typeToTypeName[type] = std::string(name); // store as std::string internally
            m_typeNameToType[std::string(name)] = type;
            return type;
        }

        return doGetType(name).value();
    }

    std::optional<std::string> getTypeName(uint16_t type) const
    {
        std::lock_guard lock(m_mutex);

        auto it = m_typeToTypeName.find(type);
        return (it != m_typeToTypeName.end()) ? std::make_optional(it->second) : std::nullopt;
    }

    std::optional<uint16_t> getType(std::string_view name) const
    {
        std::lock_guard lock(m_mutex);

        return doGetType(name);
    }

    uint16_t getTypeGuaranteedExist(std::string_view name) const
    {
        std::lock_guard lock(m_mutex);

        assert(doContains(name));
        return m_typeNameToType.at(std::string(name));
    }

    bool contains(std::string_view name) const
    {
        std::lock_guard lock(m_mutex);

        return doContains(name);
    }

    bool contains(const uint16_t &typeID) const
    {
        std::lock_guard lock(m_mutex);

        return doContains(typeID);
    }

  protected:
    mutable std::mutex m_mutex;
    uint16_t m_nextType = 1; // 0 reserved for invalid
    absl::flat_hash_map<uint16_t, std::string> m_typeToTypeName;
    absl::flat_hash_map<std::string, uint16_t> m_typeNameToType;

    bool doContains(std::string_view name) const
    {
        return m_typeNameToType.contains(std::string(name));
    }

    bool doContains(const uint16_t &type) const
    {
        return m_typeToTypeName.contains(type);
    }

    std::optional<uint16_t> doGetType(std::string_view name) const
    {
        auto it = m_typeNameToType.find(std::string(name));

        return (it != m_typeNameToType.end()) ? std::make_optional<uint16_t>(it->second) : std::nullopt;
    }
};
} // namespace star::common