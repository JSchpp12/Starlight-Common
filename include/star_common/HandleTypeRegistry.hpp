#pragma once

#include <cassert>
#include <cstdint>
#include <mutex>
#include <optional>
#include <string_view>

#include <absl/container/flat_hash_map.h>

namespace star::common
{
namespace special_types
{
// Use constexpr std::string_view instead of std::string
constexpr std::string_view ShaderTypeName = "star::shader";
constexpr std::string_view ObjectTypeName = "star::object";
constexpr std::string_view TextureTypeName = "star::texture";
constexpr std::string_view MaterialTypeName = "star::material";
constexpr std::string_view LightTypeName = "star::light";
constexpr std::string_view MapTypeName = "star::map";
constexpr std::string_view BufferTypeName = "star::buffer";
constexpr std::string_view PipelineTypeName = "star::pipeline";
constexpr std::string_view SemaphoreTypeName = "star::semaphore";
constexpr std::string_view FenceTypeName = "star::fence";
constexpr std::string_view DeviceTypeName = "star::device";
constexpr std::string_view DeviceContextTypeName = "star::systemcontext";
constexpr std::string_view SubscriberTypeName = "star::subscriber";
constexpr std::string_view ServiceCalleeTypeName = "star::service_callee";
constexpr std::string_view CommandBufferTypeName = "star::command_buffer";
constexpr std::string_view GetImageTypeName = "star::image";
constexpr std::string_view QueueTypeName = "star::queue";
} // namespace special_types

class HandleTypeRegistry
{
  public:
    HandleTypeRegistry()
    {
        initSpecialTypes();
    }

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

    static HandleTypeRegistry &instance()
    {
        static HandleTypeRegistry r;
        return r;
    }

  private:
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

    void initSpecialTypes()
    {
        using namespace special_types;
        registerType(ShaderTypeName);
        registerType(ObjectTypeName);
        registerType(TextureTypeName);
        registerType(MaterialTypeName);
        registerType(LightTypeName);
        registerType(MapTypeName);
        registerType(BufferTypeName);
        registerType(PipelineTypeName);
        registerType(SemaphoreTypeName);
        registerType(FenceTypeName);
        registerType(DeviceTypeName);
        registerType(SubscriberTypeName);
        registerType(ServiceCalleeTypeName);
        registerType(CommandBufferTypeName);
        registerType(GetImageTypeName);
    }
};

} // namespace star::common