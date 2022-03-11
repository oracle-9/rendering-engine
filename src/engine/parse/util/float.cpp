#include "engine/parse/util/float.hpp"

#include <charconv>

namespace engine::parse::util {

auto parse_float(std::string_view const s) noexcept -> std::optional<float> {
    float uninit;
    char const* const begin = s.data();
    char const* const end = begin + s.length();
    auto const [parse_end, err] = std::from_chars(begin, end, uninit);
    if (parse_end != end || err != std::errc{}) {
        return {};
    }
    return uninit;
}

} // namespace engine::util
