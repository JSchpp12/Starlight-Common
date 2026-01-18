#pragma once

#include <star_common/Handle.hpp>
#include <star_common/IEvent.hpp>

#include <cassert>
#include <functional>

namespace star::common
{
using Callback = std::function<void(const star::common::IEvent &, bool &)>;
using CallbackGetSubscriberHandleForUpdate = std::function<Handle *(void)>;
using CallbackNotifySubscriberHandleCanBeDeleted = std::function<void(const Handle &)>;

class SubscriberCallbackInfo
{
  public:
    SubscriberCallbackInfo(Callback callback, CallbackGetSubscriberHandleForUpdate callbackGetSubscriberHandleForUpdate,
                           CallbackNotifySubscriberHandleCanBeDeleted callbackNotifySubscriberHandleCanBeDeleted)
        : m_callback(callback), m_callbackGetSubscriberHandleForUpdate(callbackGetSubscriberHandleForUpdate),
          m_callbackNotifySubscriberHandleCanBeDeleted(callbackNotifySubscriberHandleCanBeDeleted)
    {
    }

    void doCallback(const star::common::IEvent &event, bool &keepAlive)
    {
        assert(m_callback && "Callback function must be defined");

        m_callback(event, keepAlive);
    }
    Handle *doCallbackGetSubscriberHandleForUpdate()
    {
        return m_callbackGetSubscriberHandleForUpdate();
    }
    void doCallbackNotifySubscriberHandleCanBeDeleted(const Handle &handle)
    {
        m_callbackNotifySubscriberHandleCanBeDeleted(handle);
    }

  private:
    Callback m_callback = nullptr;
    CallbackGetSubscriberHandleForUpdate m_callbackGetSubscriberHandleForUpdate = nullptr;
    CallbackNotifySubscriberHandleCanBeDeleted m_callbackNotifySubscriberHandleCanBeDeleted = nullptr;
};
} // namespace star::common