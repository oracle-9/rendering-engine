#pragma once

#include <concepts>

namespace util {

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

} // namespace util
