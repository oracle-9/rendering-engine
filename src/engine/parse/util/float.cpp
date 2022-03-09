#include "engine/parse/util/float.hpp"

#include <charconv>

namespace engine::parse::util {

auto parse_float(std::string_view const s) noexcept -> std::optional<float> {
    float uninit;
    auto const end = s.end();
    auto const [parse_end, err] = std::from_chars(s.begin(), end, uninit);
    if (parse_end != end || err != std::errc{}) {
        return {};
    }
    return uninit;
}

} // namespace engine::util
