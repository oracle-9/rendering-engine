#pragma once

#include "generator/err/err.hpp"

#include <brief_int.hpp>
#include <fmt/os.h>
#include <glm/vec3.hpp>
#include <result.hpp>
#include <vector>

namespace generator {

auto generate_plane(float side_len, brief_int::u32 num_divs) noexcept
    -> cpp::result<std::vector<glm::vec3>, generator_err>;

auto generate_and_print_plane(
    float side_len,
    brief_int::u32 num_divs,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, generator_err>;

}

// namespace generator
