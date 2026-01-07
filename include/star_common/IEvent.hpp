#pragma once

#include <memory>
#include <stdint.h>

namespace star::common
{
class IEvent
{
  public:
    explicit IEvent(uint16_t registeredEventType) : m_registeredEventType(std::move(registeredEventType))
    {
    }
    IEvent(const IEvent &) = default;
    IEvent &operator=(const IEvent &) = default;
    IEvent(IEvent &&) = default;
    IEvent &operator=(IEvent &&) = default;
    virtual ~IEvent() = default;

    uint16_t &getType()
    {
        return m_registeredEventType;
    }
    uint16_t getType() const
    {
        return m_registeredEventType;
    }

  private:
    uint16_t m_registeredEventType;
};
} // namespace star::common