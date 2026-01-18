#pragma once

#include "star_common/IServiceCommand.hpp"

#include <functional>

namespace star::common
{

struct ServiceCallback
{
    void *ctx{nullptr};
    void (*fn)(void *, IServiceCommand &) = nullptr;

    void operator()(IServiceCommand &cmd) const
    {
        fn(ctx, cmd);
    }

    explicit operator bool() const noexcept
    {
        return fn != nullptr;
    }
};

} // namespace star::common