#pragma once

#include "star_common/IServiceCommand.hpp"
#include "star_common/ServiceReply.hpp"

namespace star::common
{

template <typename T> class IServiceCommandWithReply : public IServiceCommand
{
  public:
    IServiceCommandWithReply() = default;
    explicit IServiceCommandWithReply(uint16_t type) : IServiceCommand(std::move(type))
    {
    }
    common::ServiceReply<T> &getReply()
    {
        return result;
    }

  private:
    common::ServiceReply<T> result;
};
} // namespace star::common