#include "engine/parse/xml/util/float_attr.hpp"

#include "engine/parse/util/float.hpp"
#include "engine/util/try.hpp"

namespace engine::parse::xml {

auto parse_float_attr(
    rapidxml::xml_node<> const* const node,
    std::string_view const attr_name
) noexcept
    -> cpp::result<float, parse_err>
{
    if (auto const* const attr
            = node->first_attribute(attr_name.data(), attr_name.length());
        attr == nullptr
    ) {
        // if attribute isn't present, assume a value of zero.
        return 0.0f;
    } else {
        return TRY_OPTION_OR(
            util::parse_float(
                std::string_view {
                    attr->value(),
                    attr->value_size(),
                }
            ),
            return cpp::failure{parse_err::malformed_num}
        );
    }
}

} // engine::parse::xml
