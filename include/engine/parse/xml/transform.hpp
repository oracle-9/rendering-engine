#ifndef ENGINE_PARSE_XML_TRANSFORM_HPP
#define ENGINE_PARSE_XML_TRANSFORM_HPP

#include "engine/parse/xml/err.hpp"
#include "engine/render/transform.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_transform(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::transform, parse_err>;

} // namespace engine::parse::xml

#endif // ENGINE_PARSE_XML_TRANSFORM_HPP
