#include "engine/parse/xml/camera/camera.hpp"

#include "engine/parse/xml/camera/projection.hpp"
#include "engine/parse/xml/util/xyz.hpp"
#include "engine/util/try.hpp"

namespace engine::parse::xml {

auto parse_camera(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::camera, parse_err>
{
    auto const* const pos_node = TRY_NULLABLE_OR(
        node->first_node("position"),
        return cpp::failure{parse_err::no_camera_pos_node}
    );
    auto const* const lookat_node = TRY_NULLABLE_OR(
        node->first_node("look_at"),
        return cpp::failure{parse_err::no_camera_lookat_node}
    );
    auto const* const up_node = TRY_NULLABLE_OR(
        node->first_node("up"),
        return cpp::failure{parse_err::no_camera_up_node}
    );
    auto const* const proj_node = TRY_NULLABLE_OR(
        node->first_node("projection"),
        return cpp::failure{parse_err::no_camera_proj_node}
    );

    return render::camera {
        .pos    = TRY_RESULT(parse_xyz(pos_node)),
        .lookat = TRY_RESULT(parse_xyz(lookat_node)),
        .up     = TRY_RESULT(parse_xyz(up_node)),
        .projection = TRY_RESULT(parse_projection(proj_node)),
    };
}

} // namespace engine::parse::xml
