#include "engine/parse/xml/camera/projection.hpp"

#include "engine/parse/xml/util/float_attr.hpp"
#include "util/try.hpp"

namespace engine::parse::xml {

auto parse_projection(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<glm::vec3, parse_err>
{
    return glm::vec3 {
        TRY_RESULT(parse_float_attr(node, "fov")),
        TRY_RESULT(parse_float_attr(node, "near")),
        TRY_RESULT(parse_float_attr(node, "far")),
    };
}

} // namespace engine::parse::xml
