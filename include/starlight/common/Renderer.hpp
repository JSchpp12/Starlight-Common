#pragma once

#include "IRenderDevice.hpp"

#include <memory>
#include <stdint.h>

namespace star::common
{
class Renderer
{
  private:
    struct RendererConcept
    {
        virtual ~RendererConcept() = default;
        virtual void prepRender(IRenderDevice &device, const uint8_t &numFramesInFlight) = 0;
        virtual void cleanupRender(IRenderDevice &device) = 0;
        virtual void frameupdate(IRenderDevice &device, const uint8_t &frameInFlightIndex) = 0;
    };

    template <typename TRenderer> struct RendererModel : public RendererConcept
    {
        TRenderer m_renderer;
        RendererModel(TRenderer renderer) : m_renderer(std::move(renderer))
        {
        }
        virtual ~RendererModel() = default;
        virtual void doPrepRender(IRenderDevice &device, const uint8_t &frameInFlightIndex) override
        {
            m_renderer.prepRender(device, frameInFlightIndex);
        }
        virtual void doCleanupRender(IRenderDevice &device) override
        {
            m_renderer.cleanupRender(device);
        }
        virtual void doFrameUpdate(IRenderDevice &device, const uint8_t &frameInFlightIndex)
        {
            m_renderer.frameUpdate(device, frameInFlightIndex);
        }
    };

    std::unique_ptr<RendererConcept> m_impl;

  public:
    template <typename TRenderer>
    Renderer(TRenderer renderer) : m_impl(std::make_unique<RendererModel>(std::move(renderer)))
    {
    }

    void prepRender(IRenderDevice &device, const uint8_t &numFramesInFlight){
      m_impl->prepRender(device, numFramesInFlight);
    }

    void cleanupRender(IRenderDevice &device){
      m_impl->cleanupRender(device);
    }

    void frameUpdate(IRenderDevice &device, const uint8_t &frameInFlightIndex){
      m_impl->frameupdate(device);
    }
};
} // namespace star::common