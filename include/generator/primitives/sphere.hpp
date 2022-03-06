#ifndef GENERATOR_PRIMITIVES_SPHERE_HPP
#define GENERATOR_PRIMITIVES_SPHERE_HPP

#include "brief_int.hpp"

#include <iosfwd>

auto generate_sphere(
    brief_int::u32 radius,
    brief_int::u32 num_slices,
    brief_int::u32 num_stacks,
    std::ofstream& output_file
) -> void;

#endif // GENERATOR_PRIMITIVES_SPHERE_HPP
