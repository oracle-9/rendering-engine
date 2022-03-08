#ifndef ENGINE_PARSE_XML_XYZ_HPP
#define ENGINE_PARSE_XML_XYZ_HPP

#include "engine/parse/xml/err.hpp"

#include <glm/vec3.hpp>
#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_xyz(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<glm::vec3, parse_err>;

} // namespace engine::parse::xml

#endif // ENGINE_PARSE_XML_XYZ_HPP
