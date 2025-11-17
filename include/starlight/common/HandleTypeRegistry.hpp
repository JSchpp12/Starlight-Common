#pragma once

#include <cassert>
#include <mutex>
#include <optional>
#include <stdint.h>
#include <string>
#include <unordered_map>

namespace star::common
{
namespace special_types
{
constexpr const std::string ShaderTypeName()
{
    return "star::shader";
}
constexpr const std::string ObjectTypeName()
{
    return "star::object";
}
constexpr const std::string TextureTypeName()
{
    return "star::texture";
}
constexpr const std::string MaterialTypeName()
{
    return "star::material";
}
constexpr const std::string LightTypeName()
{
    return "star::light";
}
constexpr const std::string MapTypeName()
{
    return "star::map";
}
constexpr const std::string BufferTypeName()
{
    return "star::buffer";
}
constexpr const std::string PipelineTypeName()
{
    return "star::pipeline";
}
constexpr const std::string SemaphoreTypeName()
{
    return "star::semaphore";
}
constexpr const std::string FenceTypeName()
{
    return "star::fence";
}
constexpr const std::string DeviceTypeName()
{
    return "star::device";
}
constexpr const std::string SubscriberTypeName()
{
    return "star::subscriber";
}
constexpr const std::string ServiceCalleeTypeName()
{
    return "star::service_callee";
}
constexpr const std::string CommandBufferTypeName()
{
    return "star::command_buffer";
}
} // namespace special_types
class HandleTypeRegistry
{
  public:
    HandleTypeRegistry()
    {
        initSpecialTypes();
    }

    // Returns a unique dynamic type ID
    uint16_t registerType(const std::string &name)
    {
        std::lock_guard lock(m_mutex);

        if (!doContains(name))
        {
            uint16_t type = m_nextType++;
            m_typeToTypeName[type] = name;
            m_typeNameToType[name] = type;
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

    std::optional<uint16_t> getType(const std::string &name) const
    {
        std::lock_guard lock(m_mutex);

        return doGetType(name);
    }

    uint16_t getTypeGuaranteedExist(const std::string &name) const
    {
        std::lock_guard lock(m_mutex);

        assert(doContains(name));
        return m_typeNameToType.find(name)->second;
    }

    bool contains(const std::string &name) const{
        std::lock_guard lock(m_mutex);

        return doContains(name);
    }

    bool contains(const uint16_t &typeID) const{
        std::lock_guard lock(m_mutex);

        return doContains(typeID);
    }

    static HandleTypeRegistry &instance()
    {
        static HandleTypeRegistry r;
        return r;
    }

  private:
    mutable std::mutex m_mutex;
    uint16_t m_nextType = 1; // 0 reserved for invalid
    std::unordered_map<uint16_t, std::string> m_typeToTypeName;
    std::unordered_map<std::string, uint16_t> m_typeNameToType;

    bool doContains(const std::string &name) const
    {
        return m_typeNameToType.contains(name);
    }

    bool doContains(const uint16_t &type) const
    {
        return m_typeToTypeName.contains(type);
    }

    std::optional<uint16_t> doGetType(const std::string &name) const
    {
        auto it = m_typeNameToType.find(name);

        return (it != m_typeNameToType.end()) ? std::make_optional<uint16_t>(it->second) : std::nullopt;
    }

    void initSpecialTypes()
    {
        registerType(special_types::ShaderTypeName());
        registerType(special_types::ObjectTypeName());
        registerType(special_types::TextureTypeName());
        registerType(special_types::MaterialTypeName());
        registerType(special_types::LightTypeName());
        registerType(special_types::MapTypeName());
        registerType(special_types::BufferTypeName());
        registerType(special_types::PipelineTypeName());
        registerType(special_types::SemaphoreTypeName());
        registerType(special_types::FenceTypeName());
        registerType(special_types::DeviceTypeName());
        registerType(special_types::SubscriberTypeName());
        registerType(special_types::ServiceCalleeTypeName());
        registerType(special_types::CommandBufferTypeName());
    }
};

} // namespace star::common