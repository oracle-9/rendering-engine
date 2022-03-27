#pragma once

#include "engine/render/world/world.hpp"

#include <GL/freeglut.h>
#include <array>
#include <glm/vec4.hpp>
#include <string_view>

namespace engine {

namespace config {

auto inline constexpr prog_name = std::string_view{"engine"};

} // namespace config

namespace render::config {

char inline constexpr WIN_TITLE[] = "engine";

enum : int {
    WIN_POS_X = 100,
    WIN_POS_Y = 100,
    WIN_WIDTH = 800,
    WIN_HEIGHT = 800,
};

static_assert(
    WIN_HEIGHT > 0,
    "WIN_HEIGHT must be greater than zero to prevent divide by zero when "
        "calculating aspect ratio."
);

auto constexpr ASPECT_RATIO
    = static_cast<double>(WIN_WIDTH)
    / static_cast<double>(WIN_WIDTH);

// pleasant black.
auto inline constexpr BG_COLOR = glm::vec4 {
    0.08f, // R
    0.08f, // G
    0.08f, // B
    1.f,   // A
};

// white.
auto inline constexpr FG_COLOR = glm::vec4{1.f, 1.f, 1.f, 1.f};

auto inline constexpr ENABLE_AXIS = false;

auto inline constexpr LINE_WIDTH = 1.f;
auto inline constexpr LINE_WIDTH_MIN = 1.f;
auto inline constexpr LINE_WIDTH_MAX = 5.f;
auto inline constexpr LINE_WIDTH_STEP = 1.f;

enum : GLenum { POLYGON_MODE = GL_LINE };

enum kb_keys : unsigned char {
    KEY_MOVE_UP = 'w',
    KEY_MOVE_LEFT = 'a',
    KEY_MOVE_DOWN = 's',
    KEY_MOVE_RIGHT = 'd',
    KEY_MOVE_UP_FAST = 'W',
    KEY_MOVE_LEFT_FAST = 'A',
    KEY_MOVE_DOWN_FAST = 'S',
    KEY_MOVE_RIGHT_FAST = 'D',
    KEY_TOGGLE_AXIS = '.',
    KEY_NEXT_POLYGON_MODE = 'm',
    KEY_THINNER_LINES = '_',
    KEY_THICKER_LINES = '*',
};

auto inline constexpr Y_AXIS_HALF_LEN = 50.f;
auto inline constexpr Z_AXIS_HALF_LEN = 50.f;
auto inline constexpr X_AXIS_HALF_LEN = 50.f;

auto inline constexpr AXIS_COLOR = std::to_array<glm::vec3>({
    {1.f, 0.f, 0.f}, // x axis (red)
    {0.f, 1.f, 0.f}, // y axis (green)
    {0.f, 0.f, 1.f}, // z axis (blue)
});

auto inline const WORLD = world {
    .camera = {
        .pos = {9., 3., 30.},
        .lookat = {0., 0., 0.},
        .up = {0., 1., 0.},
        .projection = {90., 0.5, 1000.}
    },
    .root = {},
};

enum : unsigned int { RENDER_TICK_MILLIS = 16 }; // 60 FPS

auto inline constexpr CAM_TRANSL_FACTOR = 0.06;

} // namespace render::config

} // namespace engine
