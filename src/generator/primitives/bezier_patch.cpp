#include "generator/primitives/bezier_patch.hpp"

#include "util/try.hpp"

#include <array>
#include <fstream>
#include <new>
#include <ranges>
#include <stdexcept>
#include <utility>

namespace generator {

using namespace brief_int;

struct BezierPatch {
    std::vector<std::array<usize, 16>> indices;
    std::vector<glm::vec3> ctrl_points;
};

auto static parse_patch_file(std::ifstream& patch_file) noexcept
    -> cpp::result<BezierPatch, GeneratorErr>;

auto generate_bezier_patch(
    std::ifstream& patch_input_file,
    u32 const tesselation
) noexcept -> cpp::result<std::vector<glm::vec3>, GeneratorErr>
try {
    auto const& bezier_patch = TRY_RESULT(parse_patch_file(patch_input_file));
    // TODO: implement.
    return {};
} catch (...) {}

auto generate_and_print_bezier_patch(
    std::ifstream& patch_input_file,
    u32 const tesselation,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, GeneratorErr>
try {
    auto const& vertices = TRY_RESULT(
        generate_bezier_patch(patch_input_file, tesselation)
    );
    output_file.print("{}\n", vertices.size());
    for (auto const& vertex : vertices) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    return {};
} catch (...) {
    return cpp::fail(GeneratorErr::IO_ERR);
}

#define CHECK_FILE_STATUS(file_const_ref)                                      \
    do {                                                                       \
        if (file_const_ref.bad()) {                                            \
            return cpp::fail(GeneratorErr::IO_ERR);                            \
        }                                                                      \
        if (file_const_ref.fail()) {                                           \
            return cpp::fail(GeneratorErr::MALFORMED_NUM);                     \
        }                                                                      \
    } while (0)

auto static parse_patch_file(std::ifstream& patch_file) noexcept
    -> cpp::result<BezierPatch, GeneratorErr>
try {
    using namespace brief_int::literals;

    usize num_patches;
    patch_file >> num_patches;
    CHECK_FILE_STATUS(patch_file);
    if (num_patches < 1) {
        return cpp::fail(GeneratorErr::BEZIER_LT_ONE_PATCH);
    }

    auto indices = std::vector<std::array<usize, 16>>{};
    indices.reserve(num_patches);

    auto line_buffer = std::string{};

    for (auto i = 0_uz; i < num_patches; ++i) {
        using std::ranges::views::take;
        std::array<usize, 16> patch;
        for (auto& e : patch | take(15)) {
            patch_file >> e;
            CHECK_FILE_STATUS(patch_file);
            patch_file.ignore(1, ',');
        }
        patch_file >> patch.back();
        CHECK_FILE_STATUS(patch_file);
        indices.push_back(std::move(patch));
    }

    usize num_ctrl_points;
    patch_file >> num_ctrl_points;
    CHECK_FILE_STATUS(patch_file);
    if (num_ctrl_points < 1) {
        return cpp::fail(GeneratorErr::BEZIER_LT_ONE_CTRL_POINT);
    }

    auto ctrl_points = std::vector<glm::vec3>{};
    ctrl_points.reserve(num_ctrl_points);

    for (auto i = 0_uz; i < num_ctrl_points; ++i) {
        using idx_t = glm::vec3::length_type;
        glm::vec3 ctrl_point;
        for (idx_t i = 0; i < 2; ++i) {
            patch_file >> ctrl_point[i];
            CHECK_FILE_STATUS(patch_file);
            patch_file.ignore(1, ',');
        }
        patch_file >> ctrl_point[2];
        CHECK_FILE_STATUS(patch_file);
        ctrl_points.push_back(std::move(ctrl_point));
    }

    return BezierPatch {
        .indices = std::move(indices),
        .ctrl_points = std::move(ctrl_points),
    };

} catch (std::bad_alloc const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
}

} // namespace generator
