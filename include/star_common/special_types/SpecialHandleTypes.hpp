#pragma once

#include <string_view>

namespace star::common::special_types
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
constexpr std::string_view CommandPoolTypeName = "star::command_pool";
} // namespace star::common::special_types