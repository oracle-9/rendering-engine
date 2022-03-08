#ifndef ENGINE_PARSE_XML_WORLD_HPP
#define ENGINE_PARSE_XML_WORLD_HPP

#include "engine/parse/xml/err.hpp"
#include "engine/render/world.hpp"

#include <filesystem>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_world(std::filesystem::path const& xml_filepath) noexcept
    -> cpp::result<render::world, parse_err>;

} // namespace engine::parse::xml

#endif // ENGINE_PARSE_XML_WORLD_HPP
