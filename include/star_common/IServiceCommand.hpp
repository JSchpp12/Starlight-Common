#pragma once

#include <memory>
#include <star_common/Handle.hpp>
#include <star_common/ServiceReply.hpp>
#include <stdint.h>

namespace star::common
{
class IServiceCommand
{
  public:
    IServiceCommand() = default;
    explicit IServiceCommand(uint16_t type) : m_type(std::move(type))
    {
    }
    IServiceCommand(const IServiceCommand &) = default;
    IServiceCommand &operator=(const IServiceCommand &) = default;
    IServiceCommand(IServiceCommand &&) = default;
    IServiceCommand &operator=(IServiceCommand &&) = default;
    virtual ~IServiceCommand() = default;

    void setType(uint16_t type)
    {
        m_type = std::move(type);
    }
    const uint16_t &getType() const
    {
        return m_type;
    }

  private:
    uint16_t m_type = 0;
};
} // namespace star::common