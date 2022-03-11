#pragma once

#include <glm/vec4.hpp>


namespace engine::render {

struct world;

class renderer {
  private:
    enum : int {
        WIN_POS_X = 100,
        WIN_POS_Y = 100,

        WIN_SIZ_X = 800,
        WIN_SIZ_Y = 800,
    };

    auto static constexpr BG_COLOR = glm::vec4 {
        0.08f, // R
        0.08f, // G
        0.08f, // B
        1.f,   // A
    };

    auto static constexpr WIN_TITLE = "engine";

  public:
    renderer() noexcept;

    [[noreturn]]
    auto run(world const&) noexcept -> void;
};

} // namespace engine::render
