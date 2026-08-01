#pragma once

#include <string_view>

namespace star::common::special_types
{
// Use constexpr std::string_view instead of std::string
constexpr std::string_view ShaderTypeName = "stShader";
constexpr std::string_view ObjectTypeName = "stObject";
constexpr std::string_view TextureTypeName = "stText";
constexpr std::string_view MaterialTypeName = "stMat";
constexpr std::string_view LightTypeName = "stLight";
constexpr std::string_view MapTypeName = "stMap";
constexpr std::string_view BufferTypeName = "stBuffer";
constexpr std::string_view PipelineTypeName = "stPipe";
constexpr std::string_view SemaphoreTypeName = "stSema";
constexpr std::string_view FenceTypeName = "stFence";
constexpr std::string_view DeviceTypeName = "stDev";
constexpr std::string_view DeviceContextTypeName = "stSysCon";
constexpr std::string_view SubscriberTypeName = "stSub";
constexpr std::string_view ServiceCalleeTypeName = "stScalee";
constexpr std::string_view CommandBufferTypeName = "stCmdBuff";
constexpr std::string_view GetImageTypeName = "stImg";
constexpr std::string_view QueueTypeName = "stQueue";
constexpr std::string_view CommandPoolTypeName = "stCmdPool";
constexpr std::string_view RenderPhaseTypeName = "stRPhase";
} // namespace star::common::special_types