#pragma once

#include "engine/render/world/world.hpp"

#include <glm/vec4.hpp>

namespace engine::render {

class renderer {
  private:
    world* world_ref;
    static inline struct world default_world{};

    enum : int {
        WIN_POS_X = 100,
        WIN_POS_Y = 100,

        WIN_WIDTH  = 800,
        WIN_HEIGHT = 800,
    };

    auto static constexpr BG_COLOR = glm::vec4 {
        0.08f, // R
        0.08f, // G
        0.08f, // B
        1.f,   // A
    };

    auto static constexpr WIN_TITLE = "engine";

    renderer() noexcept;

  public:
    auto static get() noexcept -> renderer&;

    auto set_world(struct world&) noexcept -> renderer&;

    [[noreturn]]
    auto run() noexcept -> void;
};

} // namespace engine::render
