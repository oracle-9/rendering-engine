#include "engine/parse/xml/group/model.hpp"

namespace engine::parse::xml {

auto parse_model(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<glm::vec3, parse_err>
{
    static_cast<void>(node);
    return {};
}

} // namespace engine::parse::xml
