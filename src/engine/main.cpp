#include "engine/module.hpp"
#include "util/pretty_print.hpp"

#include <cstdlib>
#include <fmt/core.h>
#include <string_view>

using namespace std::string_view_literals;

auto constexpr prog_name = "engine"sv;

auto main() -> int {
    auto world_result
        = engine::parse::xml::parse_world("engine_input/world.xml");

    if (world_result.has_error()) {
        pretty_print_err(
            "failed world generation with error '{}'\n",
            world_result.error()
        );
        return EXIT_FAILURE;
    }

    (*world_result).render();
}
