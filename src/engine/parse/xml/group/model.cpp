#include "engine/parse/xml/group/model.hpp"

#include "engine/util/try.hpp"

#include <brief_int.hpp>
#include <fstream>
#include <new>
#include <result.hpp>
#include <stdexcept>
#include <utility>
#include <vector>

namespace engine::parse::xml {

auto parse_model(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::model, parse_err>
try {
    using namespace brief_int;
    using namespace brief_int::literals;

    auto const* const model_filename_attr = TRY_NULLABLE_OR(
        node->first_attribute("file"),
        return cpp::failure{parse_err::no_model_filename};
    );

    char const* const model_filename = TRY_NULLABLE_OR(
        model_filename_attr->value(),
        return cpp::failure{parse_err::no_model_filename};
    );

    auto model_file = std::ifstream{model_filename};
    if (not model_file) {
        return cpp::failure{parse_err::no_model_file};
    }

    usize num_coords;
    model_file >> num_coords;
    if (model_file.bad()) {
        return cpp::failure{parse_err::io_err};
    }
    if (model_file.fail()) {
        return cpp::failure{parse_err::malformed_num};
    }

    num_coords *= 3_uz;
    auto coords = std::vector<float>{};
    coords.reserve(num_coords);

    float x, y, z;
    while (model_file >> x >> y >> z) {
        // read 3 by 3 to ensure there are no trailing coordinates.
        coords.push_back(x);
        coords.push_back(y);
        coords.push_back(z);
    }
    if (model_file.bad()) {
        return cpp::failure{parse_err::io_err};
    }
    if (model_file.fail()) {
        return cpp::failure{parse_err::malformed_num};
    }

    return render::model{std::move(coords)};

} catch (std::bad_alloc const& e) {
    return cpp::failure{parse_err::no_mem};
} catch (std::length_error const& e) {
    return cpp::failure{parse_err::no_mem};
}

} // namespace engine::parse::xml