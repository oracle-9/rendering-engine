#include "generator/primitives/plane.hpp"

#include <glm/vec2.hpp>
#include <vector>

using namespace brief_int;

auto generate_plane(
    float const side_len,
    u32 const num_divs,
    fmt::ostream& output_file
) -> void {
    using namespace brief_int::literals;

    if (num_divs == 0_u32) {
        return;
    }

    // We cache num_divs as a usize because we're going to use it in the next
    // expression.
    auto const num_divs_uz = static_cast<usize>(num_divs);

    // The total amount of vertices the cone will contain.
    auto const total_vertex_count
        = 6_uz // magic math number.
        * num_divs_uz
        * num_divs_uz;

    // We push every 2D vertex to this vector.
    // At the end of the function call, it must contain
    // total_vertex_count vertices.
    // NOTE: These vertices are stored using a CARTESIAN coordinate system
    //       (https://en.wikipedia.org/wiki/Cartesian_coordinate_system),
    //       where:
    //       - the first coordinate is x;
    //       - the second coordinate is omitted (since the plane is on the xOz
    //         plane, this value is always zero);
    //       - the third coordinate is z.
    //       Moreover, the generated triangles follow the CCW
    //       (counter-clockwise) convention.
    auto vertices_2D = std::vector<glm::vec2>{};
    vertices_2D.reserve(total_vertex_count);

    // Stores the side length of a plane division.
    auto const div_side_len = side_len / static_cast<float>(num_divs);

    // We iterate division by division.
    // The outer loop traverses the z axis and the inner the loop traverses the
    // x axis.
    for (auto row = 0_u32; row < num_divs; ++row) {
        // Stores the lower (with lower magnitude, not positionally below) z
        // coordinate of the current division.
        auto const lo_z = static_cast<float>(row) * div_side_len;

        // Stores the upper (with higher magnitude, not positionally above) z
        // coordinate of the current division.
        auto const hi_z = lo_z + div_side_len;

        for (auto col = 0_u32; col < num_divs; ++col) {
            // Stores the lower (with lower magnitude, not positionally below) x
            // coordinate of the current division.
            auto const lo_x = static_cast<float>(col) * div_side_len;

            // Stores the upper (with higher magnitude, not positionally above)
            // x coordinate of the current division.
            auto const hi_x = lo_x + div_side_len;

            // First we generate the first half of the division.
            vertices_2D.emplace_back(lo_x, lo_z);
            vertices_2D.emplace_back(lo_x, hi_z);
            vertices_2D.emplace_back(hi_x, lo_z);

            // Then we generate the second.
            vertices_2D.emplace_back(hi_x, lo_z);
            vertices_2D.emplace_back(lo_x, hi_z);
            vertices_2D.emplace_back(hi_x, hi_z);
        }
    }

    // Stores half of the plane side length.
    auto const half_side_len = side_len / 2.f;

    output_file.print("{}\n", total_vertex_count);

    // Again, since the plane is on the xOz plane, we can hardcode the y
    // coordinate to zero.
    for (auto&& vertex_2D : vertices_2D) {
        vertex_2D -= half_side_len;
        output_file.print("{} 0 {}\n", vertex_2D[0], vertex_2D[1]);
    }
}
