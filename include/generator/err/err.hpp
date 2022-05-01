#pragma once

#include <brief_int.hpp>

namespace generator {

enum class GeneratorErr : brief_int::usize {
    NO_MEM,
    IO_ERR,

    BOX_ZERO_DIVS,

    CONE_LT_THREE_SLICES,
    CONE_ZERO_STACKS,

    PLANE_ZERO_DIVS,

    SPHERE_LT_THREE_SLICES,
    SPHERE_LT_TWO_STACKS,
};

} // namespace generator
