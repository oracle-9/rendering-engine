#include "engine/parse/xml/world.hpp"

#include "engine/parse/xml/camera.hpp"
#include "engine/util/try.hpp"

#include <exception>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace {

using engine::parse::xml::parse_err;

auto open_xml_file(std::filesystem::path const& xml_filepath) noexcept
    -> cpp::result<rapidxml::file<>, parse_err>
{
    try {
        return rapidxml::file{xml_filepath.c_str()};
    } catch (std::exception const& io_err) {
        return cpp::failure{parse_err::io_err};
    }
}

} // anonymous namespace

namespace engine::parse::xml {

auto parse_world(std::filesystem::path const& xml_filepath) noexcept
    -> cpp::result<render::world, parse_err>
{
    auto input_file = TRY_RESULT(open_xml_file(xml_filepath));
    auto xml_doc = rapidxml::xml_document{};
    try {
        xml_doc.parse<rapidxml::parse_default>(input_file.data());
    } catch (rapidxml::parse_error const& xml_parse_err) {
        return cpp::failure(parse_err::syntax_err);
    }

    auto const* const world_node = TRY_NULLABLE_OR(
        xml_doc.first_node("world"),
        return cpp::failure(parse_err::no_world_node)
    );

    auto const* const camera_node = TRY_NULLABLE_OR(
        world_node->first_node("camera"),
        return cpp::failure(parse_err::no_camera_node);
    );

    return render::world {
        .camera = TRY_RESULT(parse_camera(camera_node)),
    };
}

} // namespace engine::parse::xml
