#pragma once

#include "engine/render/world/world.hpp"

#include <glm/vec4.hpp>

namespace engine::render {

class renderer {
  private:
    ///////////
    // TYPES //
    ///////////
    struct render_payload {
        struct world* world;
        double aspect_ratio;
    };

    ///////////////
    // CONSTANTS //
    ///////////////
    enum : int {
        WIN_POS_X = 100,
        WIN_POS_Y = 100,

        WIN_WIDTH  = 800,
        WIN_HEIGHT = 800,
    };

    auto static constexpr WIN_TITLE = "engine";
    auto static constexpr BG_COLOR = glm::vec4 {
        0.08f, // R
        0.08f, // G
        0.08f, // B
        1.f,   // A
    };

    ///////////////
    // INVARIANTS //
    ///////////////
    static_assert(renderer::WIN_HEIGHT > 0);

    //////////////////////
    // MEMBER_VARIABLES //
    //////////////////////
    world default_world;
    world* world_ref;
    render_payload payload;


    //////////////////////
    // MEMBER_FUNCTIONS //
    //////////////////////
    renderer() noexcept;

    /////////////
    // FRIENDS //
    /////////////
    friend auto render(void* payload) noexcept -> void;

  public:
    auto static get() noexcept -> renderer&;

    auto set_world(struct world&) noexcept -> renderer&;

    [[noreturn]]
    auto run() noexcept -> void;
};

} // namespace engine::render
