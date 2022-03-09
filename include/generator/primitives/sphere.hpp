#pragma once

#include <brief_int.hpp>
#include <fmt/os.h>
#include <cmath>

auto generate_sphere(
    brief_int::u32 radius,
    brief_int::u32 num_slices,
    brief_int::u32 num_stacks,
    fmt::ostream& output_file
) -> void;
