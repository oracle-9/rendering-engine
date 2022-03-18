#pragma once

#include <brief_int.hpp>
#include <fmt/os.h>

auto generate_plane(
    float side_len,
    brief_int::u32 num_divs,
    fmt::ostream& output_file
) -> void;
