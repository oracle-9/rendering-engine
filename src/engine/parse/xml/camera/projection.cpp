#include "engine/parse/xml/camera/projection.hpp"

#include "engine/parse/xml/util/number_attr.hpp"
#include "util/try.hpp"

namespace engine::parse::xml {

auto parse_projection(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<glm::dvec3, parse_err>
{
    return glm::vec3 {
        TRY_RESULT(parse_number_attr<double>(node, "fov")),
        TRY_RESULT(parse_number_attr<double>(node, "near")),
        TRY_RESULT(parse_number_attr<double>(node, "far")),
    };
}

} // namespace engine::parse::xml
