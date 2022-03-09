#pragma once

#include <optional>
#include <string_view>

namespace engine::parse::util {

auto parse_float(std::string_view s) noexcept -> std::optional<float>;

} // namespace engine::util
