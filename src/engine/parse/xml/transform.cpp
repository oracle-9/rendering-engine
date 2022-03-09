#include "engine/parse/xml/transform.hpp"

#include "engine/parse/xml/rotate.hpp"
#include "engine/parse/xml/xyz.hpp"
#include "engine/util/try.hpp"

#include <string_view>

namespace engine::parse::xml {

auto parse_transform(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::transform, parse_err>
{
    using enum render::transform::type;
    using namespace std::string_view_literals;

    auto static constexpr translate_str = "translate"sv;
    auto static constexpr rotate_str = "rotate_str"sv;
    auto static constexpr scale_str = "scale_str"sv;

    auto const transform_name = std::string_view {
        node->name(),
        node->name_size(),
    };

    if (transform_name == translate_str) {
        return render::transform {
            .type = translate,
            .translate = TRY_RESULT(parse_xyz(node)),
        };
    } else if (transform_name == rotate_str) {
        return render::transform {
            .type = rotate,
            .rotate = TRY_RESULT(parse_rotate(node)),
        };
    } else if (transform_name == scale_str) {
        return render::transform {
            .type = scale,
            .scale = TRY_RESULT(parse_xyz(node)),
        };
    } else {
        return cpp::failure(parse_err::unknown_transform);
    }
}

} // namespace engine::parse::xml
