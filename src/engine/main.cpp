#include "engine/module.hpp"

#include <cstdlib>
#include <fmt/core.h>

auto main() -> int {
    auto world_result
        = engine::parse::xml::parse_world("engine_input/world.xml");

    if (world_result.has_error()) {
        fmt::print(
            "failed world generation with error {}\n",
            world_result.error()
        );
        return EXIT_FAILURE;
    }

    (*world_result).render();
}
