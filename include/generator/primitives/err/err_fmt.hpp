#pragma once

#include "generator/primitives/err/err.hpp"

#include <array>
#include <fmt/format.h>
#include <string_view>
#include <type_traits>

template <>
struct fmt::formatter<generator::generator_err> {
    using generator_err = ::generator::generator_err;

    auto constexpr parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto constexpr format(generator_err const err, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", [err] {
            using namespace brief_int::literals;

            auto static constexpr to_string = std::to_array<std::string_view>({
                "ran out of memory",
                "input/output error",

                "attempted to generate a box with zero divisions",

                "attempted to generate a cone with less than three slices",
                "attempted to generate a cone with zero stacks",

                "attempted to generate a plane with zero divisions",

                "attempted to generate a sphere with less than three slices",
                "attempted to generate a sphere with less than two stacks",
            });

            auto const idx
                = static_cast<std::underlying_type_t<generator_err>>(err);

            if (idx >= 0_uz || idx <= 7_uz) {
                return to_string[idx];
            }

            __builtin_unreachable();
        }());
    }
};
