#include "engine/parse/xml/rotate.hpp"

#include "engine/parse/xml/float_attr.hpp"
#include "engine/util/try.hpp"

namespace engine::parse::xml {

auto parse_rotate(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<glm::vec4, parse_err>
{
    return glm::vec4 {
        TRY_RESULT(parse_float_attr(node, "angle")),
        TRY_RESULT(parse_float_attr(node, "x")),
        TRY_RESULT(parse_float_attr(node, "y")),
        TRY_RESULT(parse_float_attr(node, "z")),
    };
}

} // namespace engine::parse::xml
