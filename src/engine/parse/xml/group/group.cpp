#include "engine/parse/xml/group/group.hpp"

#include "engine/parse/xml/group/model/model_list.hpp"
#include "engine/parse/xml/group/transform/transform_list.hpp"
#include "util/try.hpp"

#include <new>
#include <stdexcept>
#include <string_view>

namespace engine::parse::xml {

// TODO: Implement non-recursively.
auto parse_group(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::group, parse_err>
try {
    using namespace std::string_view_literals;

    auto static constexpr transform_str = "transform"sv;
    auto static constexpr models_str = "models"sv;
    auto static constexpr group_str = "group"sv;

    auto root = render::group{};

    for (
        auto const* subnode = node->first_node();
        subnode != nullptr;
        subnode = subnode->next_sibling()
    ) {
        auto const subnode_name = std::string_view {
            node->name(),
            node->name_size(),
        };

        if (subnode_name == transform_str) {
            root.transforms = TRY_RESULT(parse_transform_list(subnode));
        } else if (subnode_name == models_str) {
            root.models = TRY_RESULT(parse_model_list(subnode));
        } else if (subnode_name == group_str) {
            root.children.push_back(TRY_RESULT(parse_group(subnode)));
        } else {
            return cpp::failure{parse_err::unknown_group_subnode};
        }
    }

    return root;

} catch (std::bad_alloc const&) {
    return cpp::failure{parse_err::no_mem};
} catch (std::length_error const&) {
    return cpp::failure{parse_err::no_mem};
}

} // namespace engine::parse::xml
