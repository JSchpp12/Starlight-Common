#pragma once

#include "star_common/special_types/SpecialHandleTypes.hpp"
#include "star_common/TypeRegistry.hpp"

namespace star::common
{

class HandleTypeRegistry : public TypeRegistry
{
  public:
    HandleTypeRegistry()
    {
        initSpecialTypes();
    }

    static HandleTypeRegistry &instance()
    {
        static HandleTypeRegistry r;
        return r;
    }

  private:
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