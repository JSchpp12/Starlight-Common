#pragma once

#include "EventBus.hpp"
#include "IDeviceContext.hpp"
#include "FrameTracker.hpp"

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
        virtual void doPrepRender(IDeviceContext &device) = 0;
        virtual void doCleanupRender(IDeviceContext &device) = 0;
        virtual void doFrameUpdate(IDeviceContext &device) = 0;
    };

    template <typename T> struct RendererModel : public RendererConcept
    {
        T m_renderer;

        template <typename U>
        explicit RendererModel(U &&renderer) : m_renderer(std::forward<U>(renderer))
        {
        }

        virtual ~RendererModel() = default;
        virtual void doPrepRender(IDeviceContext &device) override
        {
            m_renderer.prepRender(device);
        }
        virtual void doCleanupRender(IDeviceContext &device) override
        {
            m_renderer.cleanupRender(device);
        }
        virtual void doFrameUpdate(IDeviceContext &device) override
        {
            m_renderer.frameUpdate(device);
        }
    };

    std::unique_ptr<RendererConcept> m_impl;

  public:
    template <typename TRenderer>
    explicit Renderer(TRenderer &&renderer)
        : m_impl(std::make_unique<RendererModel<std::decay_t<TRenderer>>>(std::forward<TRenderer>(renderer)))
    {
    }
    
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&other) : m_impl(std::move(other.m_impl)){};
    Renderer &operator=(Renderer &&other)
    {
        if (this != &other)
        {
            m_impl = std::move(other.m_impl);
        }
        return *this;
    };

    void prepRender(IDeviceContext &device)
    {
        m_impl->doPrepRender(device);
    }

    void cleanupRender(IDeviceContext &device)
    {
        m_impl->doCleanupRender(device);
    }

    void frameUpdate(IDeviceContext &device)
    {
        m_impl->doFrameUpdate(device);
    }

    template <typename T> T *getRaw() noexcept
    {
        if (!m_impl)
            return nullptr; // moved-from guard
        // Assumes caller supplies the exact stored type T.
        return &static_cast<RendererModel<T> *>(m_impl.get())->m_renderer;
    }

    template <typename T> const T *getRaw() const noexcept
    {
        if (!m_impl)
            return nullptr;
        return &static_cast<const RendererModel<T> *>(m_impl.get())->m_renderer;
    }
};
} // namespace star::common