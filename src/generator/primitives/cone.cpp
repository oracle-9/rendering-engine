#include "generator/primitives/cone.hpp"

#include "util/coord_conv.hpp"

#include <algorithm>
#include <glm/vec3.hpp>
#include <numbers>
#include <vector>

using namespace brief_int;

auto generate_cone(
    float const radius,
    float const height,
    u32 const num_slices,
    u32 const num_stacks,
    fmt::ostream& output_file
) -> void {
    using namespace brief_int::literals;

    if (num_slices <= 2_u32) {
        return;
    }

    if (num_stacks == 0_u32) {
        return;
    }

    // We cache num_stacks as a float because we're going to use it in some
    // expressions ahead.
    auto const num_stacks_f = static_cast<float>(num_stacks);

    // Stores the angle, in radians, of a slice.
    // Radians are used instead of degrees to allow easy conversion from
    // cylindrical coordinates to cartesian coordinates, at the end of the
    // function.
    // This specific order of operations *should* provide the best
    // approximation, since 2.f and num_stacks_f are both integers.
    auto const slice_angle
        = std::numbers::pi_v<float>
        * (2.f / static_cast<float>(num_slices));

    // Stores a factor with which to compute the radius at a given stack
    // separator. This value is equivalent to the radius of the last (upper)
    // stack.
    auto const radius_factor = radius / num_stacks_f;

    // Stores the height of a stack.
    auto const stack_height = height / num_stacks_f;

    // We cache this value because we're going to use it as an iteration limit
    // in the inner loop.
    // This expression won't wrap around since num_stacks cannot be lower
    // than 1, which we have asserted already.
    auto const num_stacks_minus_one = num_stacks - 1_u32;

    // Stores the distance from the base of the cone to the last stack separator.
    auto const top_height = height - stack_height;

    // The "pointy end" of the cone
    // (https://en.wikipedia.org/wiki/Apex_(geometry)).
    // We cache this value because we're going to use it a lot when generating
    // the last stack of the cone.
    auto const apex = glm::vec3{0.f, height, 0.f};

    // The total amount of vertices the cone will contain.
    auto const total_vertex_count
        = 6_uz // magic math number.
        * static_cast<usize>(num_slices)
        * static_cast<usize>(num_stacks);

    // We push every vertex to this vector.
    // At the end of the function call, it must contain
    // total_vertex_count vertices.
    // NOTE: These vertices are stored using a CYLINDRICAL coordinate system
    //       (https://en.wikipedia.org/wiki/Cylindrical_coordinate_system),
    //       where:
    //       - the first coordinate is the radial distance aka radius;
    //       - the second coordinate is the azimuth aka angular position in
    //         RADIANS;
    //       - the third coordinate is the altitude aka height.
    //       In order to forward these vertices to OpenGL, they need to be
    //       converted to a CARTESIAN coordinate system.
    //       Moreover, the generated triangles follow the CCW
    //       (counter-clockwise) convention.
    auto vertices = std::vector<glm::vec3>{};
    vertices.reserve(total_vertex_count);

    // We iterate slice by slice, stack by stack.
    // i represents the current slice.
    for (auto i = 0_u32; i < num_slices; ++i) {
        // Stores the accumulated angle, i.e. the sum of all angles from the
        // first slice up until this slice.
        auto const curr_angle = static_cast<float>(i) * slice_angle;

        // Same, but for the next slice.
        auto const next_angle = static_cast<float>(i + 1_u32) * slice_angle;

        // The base of the cone is composed of num_slices triangles.
        // The next 3 lines of code generate the base of the current slice,
        // i.e. the i'th triangle that constitutes the base of the cone.
        //
        // First we generate the center of the base of the cone, which is
        // (conveniently) the point (0, 0, 0).
        vertices.emplace_back(0.f, 0.f, 0.f);
        vertices.emplace_back(radius, 0.f, curr_angle);
        vertices.emplace_back(radius, 0.f, next_angle);

        // Next, we generate the "body" of the slice, that is, the "walls" of
        // the slice that constitute the "walls" of the cone.
        // We generate these vertices up until (excluding) num_stacks, since the
        // last stack must be drawn differently than the other stack.
        //
        // These first walls are represented by
        // quads (https://en.wikipedia.org/wiki/Quadrilateral), i.e.
        // 2 triangles joined, while the last (upper) walls of the cone are
        // simply triangles.

        // j represents the current stack.
        for (auto j = 0_u32; j < num_stacks_minus_one; ++j) {
            // We cache j as a float because we're going to use it more than
            // once.
            auto const j_f = static_cast<float>(j);

            // We do the same of j + 1.
            auto const j_plus_1_f = static_cast<float>(j + 1_u32);

            // Stores the distance FROM the normal that intersects
            // the center of the base of the cone TO any vertex that constitutes
            // the current stack separator.
            auto const curr_radius = radius - j_f * radius_factor;

            // The same as the above, but for the next stack separator, where
            // 'the next stack' means 'the stack above this stack (in the
            // vertical y axis)'.
            auto const next_radius = radius - j_plus_1_f * radius_factor;

            // Stores the distance FROM the base of the cone TO any vertex that
            // constitutes the current stack separator.
            auto const curr_height = j_f * stack_height;

            // The same as the above, but for the next stack separator, where
            // 'the next stack' means 'the stack above this stack (in the
            // vertical y axis)'.
            auto const next_height = j_plus_1_f * stack_height;

            // First we generate the first half of the slice wall.
            vertices.emplace_back(curr_radius, curr_height, curr_angle);
            vertices.emplace_back(next_radius, next_height, curr_angle);
            vertices.emplace_back(next_radius, next_height, next_angle);

            // Then we generate the second.
            vertices.emplace_back(curr_radius, curr_height, curr_angle);
            vertices.emplace_back(next_radius, next_height, next_angle);
            vertices.emplace_back(curr_radius, curr_height, next_angle);
        }

        // Finally, we generate the upper wall of the slice.
        // Again, this is just a triangle, not a quad.
        // Reminder that radius_factor is equivalent to the radius of the last
        // (upper) stack.
        vertices.push_back(apex);
        vertices.emplace_back(radius_factor, top_height, next_angle);
        vertices.emplace_back(radius_factor, top_height, curr_angle);
    }

    output_file.print("{}\n", total_vertex_count);

    // We need to make sure the vertices are represented with a cartesian
    // coordinate system before we forward them to OpenGL.
    for (auto&& vertex : vertices) {
        util::to_cartesian_inplace(vertex);
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
}
