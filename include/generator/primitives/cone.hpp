#ifndef GENERATOR_PRIMITIVES_CONE_HPP
#define GENERATOR_PRIMITIVES_CONE_HPP

#include "brief_int.hpp"
#include "fmt/os.h"

auto generate_cone(
    brief_int::u32 radius,
    brief_int::u32 height,
    brief_int::u32 num_slices,
    brief_int::u32 num_stacks,
    fmt::ostream& output_file
) -> void;

#endif // GENERATOR_PRIMITIVES_CONE_HPP
