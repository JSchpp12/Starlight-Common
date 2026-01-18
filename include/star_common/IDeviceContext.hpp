#pragma once

#include "EventBus.hpp"
#include "IRenderDevice.hpp"

namespace star::common
{
class IDeviceContext
{
  public:
    virtual ~IDeviceContext() = default;
    virtual EventBus &getEventBus() = 0;
    virtual IRenderDevice &getDevice() = 0;
};
} // namespace star::common