#pragma once

#include <absl/synchronization/notification.h>
#include <memory>

namespace star::common
{
template <class T> class ServiceReply
{
    T m_value;

  public:
    void set(T value)
    {
        m_value = std::move(value); 
    }

    T &get()
    {
        return m_value;
    }
};
template <class T> class ServiceReplayLazy
{
  private:
    struct State
    {
        absl::Notification done;
        T value;
    };

    std::shared_ptr<State> m_state;

  public:
    ServiceReplayLazy() : m_state(std::make_shared<State>())
    {
    }

    void set(T v)
    {
        m_state->value = std::move(v);
        m_state->done.Notify();
    }

    /// <summary>
    /// Wait for response from service
    /// </summary>
    /// <returns></returns>
    T wait() const
    {
        m_state->done.WaitForNotification();
        return std::move(m_state->value);
    }

    bool ready() const
    {
        return m_state->done.HasBeenNotified();
    }

    const T &peek() const
    {
        return m_state->value;
    }

    bool valid() const
    {
        return static_cast<bool>(m_state);
    }
};
} // namespace star::common