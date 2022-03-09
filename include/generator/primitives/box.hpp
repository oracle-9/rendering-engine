#pragma once

#include <brief_int.hpp>
#include <fmt/os.h>

auto generate_box(
    brief_int::u32 num_units,
    brief_int::u32 grid_len,
    fmt::ostream& output_file
) -> void;
