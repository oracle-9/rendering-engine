#pragma once

#include <brief_int.hpp>

namespace generator {

// NOTE: changing the order of the enumerators requires a change in the
// string formatting implementation.
enum class generator_err : brief_int::usize {
    no_mem,
    io_err,

    box_zero_divs,

    cone_lt_three_slices,
    cone_zero_stacks,

    plane_zero_divs,

    sphere_lt_three_slices,
    sphere_lt_two_stacks,
};

} // namespace generator
