#pragma once

#include "engine/render/world/world.hpp"

#include <GL/freeglut.h>
#include <array>
#include <glm/vec4.hpp>
#include <string_view>

namespace engine {

namespace config {
    auto constexpr prog_name = std::string_view{"engine"};
} // namespace config

namespace render::config {

char constexpr WIN_TITLE[] = "engine";

enum : int {
    DEFAULT_WIN_POS_X = 100,
    DEFAULT_WIN_POS_Y = 100,

    DEFAULT_WIN_WIDTH = 800,
    DEFAULT_WIN_HEIGHT = 800,
};
static_assert(
    DEFAULT_WIN_HEIGHT > 0,
    "DEFAULT_WIN_HEIGHT must be greater than zero to prevent divide by zero"
        " when calculating aspect ratio."
);

auto constexpr DEFAULT_ASPECT_RATIO
    = static_cast<double>(DEFAULT_WIN_WIDTH)
    / static_cast<double>(DEFAULT_WIN_WIDTH);

auto constexpr DEFAULT_BG_COLOR = glm::vec4 {
    0.08f, // R
    0.08f, // G
    0.08f, // B
    1.f,   // A
};

auto constexpr DEFAULT_FG_COLOR = glm::vec4{1.f, 1.f, 1.f, 1.f}; // white

enum : GLenum { DEFAULT_POLYGON_MODE = GL_LINE };

enum kb_keys : unsigned char {
    KEY_MOVE_UP = 'w',
    KEY_MOVE_LEFT = 'a',
    KEY_MOVE_DOWN = 's',
    KEY_MOVE_RIGHT = 'd',
    KEY_NEXT_POLYGON_MODE = 'm',
};

auto constexpr Y_AXIS_HALF_LEN = 50.f;
auto constexpr Z_AXIS_HALF_LEN = 50.f;
auto constexpr X_AXIS_HALF_LEN = 50.f;

auto constexpr AXIS_COLOR = std::to_array<glm::vec3>({
    {1.f, 0.f, 0.f}, // x axis (red)
    {0.f, 1.f, 0.f}, // y axis (green)
    {0.f, 0.f, 1.f}, // z axis (blue)
});


auto const DEFAULT_WORLD = world {
    .camera = {
        .pos = {9., 3., 30.},
        .lookat = {0., 0., 0.},
        .up = {0., 1., 0.},
        .projection = {90., 0.5, 1000.}
    },
    .root = {},
};

enum : unsigned int { RENDER_TICK_MILLIS = 16 }; // 60 FPS

auto constexpr CAM_TRANSL_FACTOR = 0.06;

} // namespace render::config

} // namespace engine
