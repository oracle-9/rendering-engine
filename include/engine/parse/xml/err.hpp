#ifndef ENGINE_PARSE_XML_ERR_HPP
#define ENGINE_PARSE_XML_ERR_HPP

namespace engine::parse::xml {

enum class parse_err {
    io_err,
    syntax_err,
    malformed_num,

    no_world_node,

    no_camera_node,
    no_camera_pos_node,
    no_camera_lookat_node,
    no_camera_up_node,
    no_camera_proj_node,

    unknown_transform,
};

} // namespace engine::parse::xml

#endif // ENGINE_PARSE_XML_ERR_HPP
