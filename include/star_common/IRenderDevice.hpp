#pragma once

namespace star::common
{

class IRenderDevice
{
  public:
    using NativeHandle = void *;

    virtual ~IRenderDevice() = default;
    virtual NativeHandle getNativeDevice() = 0;
};

} // namespace star::common