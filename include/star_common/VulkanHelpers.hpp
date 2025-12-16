#pragma once

#include <starlight/common/IRenderDevice.hpp>

namespace star::common
{

struct VkDevice_T;
using VkDevice = VkDevice_T *;

// inline VkDevice getVulkanDevice(const IRenderDevice &rd) noexcept
// {
//     return static_cast<VkDevice>(rd.getNativeDevice());
// }

} // namespace star::common