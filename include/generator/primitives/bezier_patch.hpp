#pragma once

#include "generator/err/err.hpp"

#include <brief_int.hpp>
#include <fmt/os.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <iosfwd>
#include <result.hpp>
#include <vector>

namespace generator {

auto generate_bezier_patch(
    std::ifstream& patch_input_file,
    brief_int::u32 tesselation
) noexcept -> cpp::result<std::vector<glm::vec3>, GeneratorErr>;

auto generate_and_print_bezier_patch(
    std::ifstream& patch_input_file,
    brief_int::u32 tesselation,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, GeneratorErr>;

} // namespace generator
