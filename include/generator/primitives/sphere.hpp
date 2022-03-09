#ifndef GENERATOR_PRIMITIVES_SPHERE_HPP
#define GENERATOR_PRIMITIVES_SPHERE_HPP
#define _USE_MATH_DEFINES

#include <cmath>
#include "brief_int.hpp"
#include "fmt/os.h"

auto generate_sphere(
    brief_int::u32 radius,
    brief_int::u32 num_slices,
    brief_int::u32 num_stacks,
    fmt::ostream& output_file
) -> void;

#endif // GENERATOR_PRIMITIVES_SPHERE_HPP
