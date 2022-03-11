#pragma once

#include <glm/vec4.hpp>
#include <string_view>

namespace engine {
    namespace config {
        auto constexpr prog_name = std::string_view{"engine"};
    } // namespace config

    namespace render::config {
        auto static constexpr WIN_TITLE = "engine";

        enum : int {
            DEFAULT_WIN_POS_X = 100,
            DEFAULT_WIN_POS_Y = 100,

            DEFAULT_WIN_WIDTH  = 800,
            DEFAULT_WIN_HEIGHT = 800,
        };
        static_assert(
            DEFAULT_WIN_HEIGHT > 0,
            "DEFAULT_WIN_HEIGHT must be greater than zero to prevent divide by"
            " zero when calculating aspect ratio."
        );

        auto static constexpr DEFAULT_BG_COLOR = glm::vec4 {
            0.08f, // R
            0.08f, // G
            0.08f, // B
            1.f,   // A
        };
    } // namespace render::config
} // namespace engine
