#pragma once

namespace engine::parse::xml {

enum class ParseErr {
    NO_MEM,
    IO_ERR,

    SYNTAX_ERR,
    MALFORMED_NUM,

    NO_WORLD_NODE,
    NO_CAMERA_NODE,
    NO_GROUP_NODE,

    NO_CAMERA_POS_NODE,
    NO_CAMERA_LOOKAT_NODE,
    NO_CAMERA_UP_NODE,
    NO_CAMERA_PROJ_NODE,

    UNKNOWN_GROUP_CHILD_NODE,

    UNKNOWN_TRANSFORM,

    NO_MODEL_FILENAME,
    AMBIGUOUS_MODEL_EXT,
    NO_MODEL_FILE,
    OBJ_LOADER_ERR,
};

} // namespace engine::parse::xml
