#ifndef GENERATOR_PRIMITIVES_BOX_HPP
#define GENERATOR_PRIMITIVES_BOX_HPP

#include "brief_int.hpp"

#include <iosfwd>

auto generate_box(
    brief_int::u32 num_units,
    brief_int::u32 grid_len,
    std::ofstream& output_file
) -> void;

#endif // GENERATOR_PRIMITIVES_BOX_HPP
