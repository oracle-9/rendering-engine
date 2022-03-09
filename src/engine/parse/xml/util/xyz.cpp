#include "engine/parse/xml/util/xyz.hpp"

#include "engine/parse/xml/util/float_attr.hpp"
#include "engine/util/try.hpp"

namespace engine::parse::xml {

auto parse_xyz(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<glm::vec3, parse_err>
{
    return glm::vec3 {
        TRY_RESULT(parse_float_attr(node, "x")),
        TRY_RESULT(parse_float_attr(node, "y")),
        TRY_RESULT(parse_float_attr(node, "z")),
    };
}

} // namespace engine::parse::xml
