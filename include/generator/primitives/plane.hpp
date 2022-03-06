#ifndef GENERATOR_PRIMITIVES_PLANE_HPP
#define GENERATOR_PRIMITIVES_PLANE_HPP

#include "brief_int.hpp"

#include <iosfwd>

auto generate_plane(
    brief_int::u32 len,
    brief_int::u32 num_divs,
    std::ofstream& output_file
) -> void;

#endif // GENERATOR_PRIMITIVES_PLANE_HPP
