#pragma once

#include <brief_int.hpp>

namespace generator {

// NOTE: changing the order of the enumerators requires a change in the
// string formatting implementation.
enum class generator_err : brief_int::usize {
    no_mem,
    io_err,
};

} // namespace generator
