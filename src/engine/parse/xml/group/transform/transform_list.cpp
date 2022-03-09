#include "engine/parse/xml/group/transform/transform_list.hpp"

#include "engine/parse/xml/group/transform/transform.hpp"
#include "engine/util/try.hpp"

namespace engine::parse::xml {

auto parse_transform_list(rapidxml::xml_node<> const* const node)
    -> cpp::result<std::vector<render::transform>, parse_err>
{
    auto transform_list = std::vector<render::transform>{};

    for (
        auto const* transform = node->first_node();
        transform != nullptr;
        transform = transform->next_sibling()
    ) {
        transform_list.push_back(TRY_RESULT(parse_transform(node)));
    }

    return transform_list;
}

} // engine::parse::xml
