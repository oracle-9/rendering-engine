#include "generator/primitives/cone.hpp"

#include <fstream>

using namespace brief_int;

auto generate_cone(
    u32 radius,
    u32 height,
    u32 num_slices,
    u32 num_stacks,
    std::ofstream& output_file
) -> void {
    static_cast<void>(radius);
    static_cast<void>(height);
    static_cast<void>(num_slices);
    static_cast<void>(num_stacks);
    static_cast<void>(output_file);
}
