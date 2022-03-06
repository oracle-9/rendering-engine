#include "fmt/core.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

namespace xml = rapidxml;

auto main() -> int {
    auto input_file = xml::file{"generator_input/catalog.xml"};
    auto xml_doc = xml::xml_document{};
    xml_doc.parse<xml::parse_default>(input_file.data());

    auto* const root = xml_doc.first_node("catalog");
    if (not root) {
        return 1;
    }

    fmt::print("root name: {}\n", root->name());

    for (
        auto const* book = root->first_node();
        book != nullptr;
        book = book->next_sibling()
    ) {
        fmt::print("Book<id: {}> {{", book->first_attribute("id")->value());
        for (
            auto const* prop = book->first_node();
            prop != nullptr;
            prop = prop->next_sibling()
        ) {
            fmt::print("\n    {}: {},", prop->name(), prop->value());
        }
        fmt::print("\n}},\n");
    }
}
