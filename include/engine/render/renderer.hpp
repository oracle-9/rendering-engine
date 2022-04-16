#pragma once

namespace engine::render {

// Forward declarations.
struct camera;
struct world;
class renderer;

auto get() -> renderer&;

class renderer {
  private:
    auto friend get() -> renderer&;
    renderer();

  public:
    auto set_world(world& world) noexcept -> renderer&;

    auto set_camera(camera& camera) noexcept -> renderer&;

    auto run() noexcept -> void;
};

} // namespace engine::render
