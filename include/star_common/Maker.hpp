#pragma once

#include <concepts>
#include <cstddef>
#include <expected>
#include <stdexcept>
#include <string>

namespace star::common
{
/// @brief Requirements for the type produced by a Maker.
///
/// Makers construct values in place over the caller's storage using placement new, discarding whatever
/// the destination held before. For that protocol to be safe the produced type must be:
///  - default_initializable: callers can pre-fill the destination (e.g. std::vector<T>(n),
///    std::array<T, n>) with default values that make() may overwrite without destroying;
///  - destructible: every slot of a pre-filled destination remains destroyable if make() fails after
///    constructing only a prefix, so normal container cleanup is always well defined.
template <typename T>
concept Makeable = std::default_initializable<T> && std::destructible<T>;

/// @brief Base for objects that build one or more values of type T.
/// @tparam T The type produced by the maker. Must satisfy Makeable.
/// @tparam E The error type reported when building fails.
template <Makeable T, typename E> class Maker
{
  public:
    virtual ~Maker() = default;

    /// @brief Build num values of type T in place at dst.
    ///
    /// Values are written with placement new; whatever dst currently holds is discarded without
    /// destruction. Pass pre-filled storage (e.g. std::vector<T>(num) or std::array<T, num>) so that
    /// every slot stays destroyable if make() returns an error after constructing only some of the
    /// values. On error an unspecified prefix of dst holds newly constructed values; the remaining
    /// slots hold whatever was there before the call. If the error type reports how many values were
    /// built, destroy only those slots when cleaning up raw storage.
    ///
    /// @param dst Pointer to storage for num values of type T.
    /// @param num Number of values to build.
    /// @return The number of values built, or the error that occurred.
    [[nodiscard]] virtual std::expected<size_t, E> make(T *dst, size_t num) noexcept = 0;

    /// @brief Build a single value and return it, throwing if make() reports an error.
    ///
    /// Convenience for callers that hold one configured maker and build values lazily, such as object pools.
    [[nodiscard]] T operator()()
    {
        T value;
        const auto result = make(&value, 1);
        if (!result)
        {
            ThrowMakeError(result.error());
        }

        return value;
    }

  private:
    [[noreturn]] static void ThrowMakeError(const E &error)
    {
        if constexpr (requires { std::string(error.detail); })
        {
            throw std::runtime_error(error.detail);
        }
        else
        {
            throw std::runtime_error("Maker failed to build a value");
        }
    }
};
} // namespace star::common