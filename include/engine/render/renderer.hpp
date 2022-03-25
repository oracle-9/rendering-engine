#pragma once

namespace engine::render {

// Forward declarations.
struct world;
class renderer;

auto launch() -> renderer&;

class renderer {
  private:
    auto friend launch() -> renderer&;
    renderer();

  public:
    auto set_world(world& world) noexcept -> renderer&;

    [[noreturn]]
    auto run() noexcept -> void;
};

} // namespace engine::render
