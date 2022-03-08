#ifndef ENGINE_UTIL_PARSE_FLOAT_HPP
#define ENGINE_UTIL_PARSE_FLOAT_HPP

#include <optional>
#include <string_view>

namespace engine::util {

auto parse_float(std::string_view s) noexcept -> std::optional<float>;

} // namespace engine::util

#endif // ENGINE_UTIL_PARSE_FLOAT_HPP
