#include "brief_int.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
#include "generator/primitives/module.hpp"
#include "generator/util/pretty_print.hpp"

#include <cerrno>
#include <charconv>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

using namespace brief_int;
using namespace std::string_view_literals;

enum class cli_option {
    help,
    sphere,
    box,
    cone,
    plane,
};

auto constexpr prog_name = "generator"sv;

auto const options = std::unordered_map<std::string_view, cli_option>{
    {"-h",     cli_option::help},
    {"--help", cli_option::help},
    {"sphere", cli_option::sphere},
    {"box",    cli_option::box},
    {"cone",   cli_option::cone},
    {"plane",  cli_option::plane},
};

auto parse_u32(char const* const s) -> u32 {
    u32 uninit;
    char const* const end = s + std::strlen(s);
    auto const [parse_end, err] = std::from_chars(s, end, uninit);
    if (parse_end != end || err != std::errc()) {
        throw std::invalid_argument{
            fmt::format(
                "failed parsing '{}' into u32", std::string_view{s, end}
            )
        };
    }
    return uninit;
}

auto check_num_args(int const expected, int const actual) -> void {
    if (expected != actual) {
        throw std::invalid_argument{
            fmt::format("expected {} arguments, but got {}", expected, actual)
        };
    }
}

auto open_output_file(char const* const output_filename) -> std::ofstream {
    auto output_file = std::ofstream(output_filename);
    output_file.exceptions(output_file.failbit);
    return output_file;
}

auto display_help() -> void {
    using namespace fmt::literals;
    fmt::print(
        "Usage:\n"
        "    {prog} (-h | --help)\n"
        "        Display this message.\n"
        "\n"
        "\n"
        "    {prog} (sphere | box | cone | plane) <args>... <output_file>\n"
        "        Draw the specified primitive and store the resulting\n"
        "        vertices in a file named <output_file>.\n"
        "\n"
        "    sphere <radius> <num_slices> <num_stacks>\n"
        "        Generate a sphere with radius <radius>, <num_slices> slices\n"
        "        and <num_stacks> stacks.\n"
        "\n"
        "    box <num_units> <grid_len>\n"
        "        Generate a box with <num_units> units, where each side is\n"
        "        divided in a <grid_len>x<grid_len> grid.\n"
        "\n"
        "    cone <radius> <height> <num_slices> <num_stacks>\n"
        "        Generate a cone with radius <radius>, height <height>,\n"
        "        <num_slices> slices and <num_stacks> stacks.\n"
        "\n"
        "    plane <len> <num_divs>\n"
        "        Generate a plane with <len> units in length, and <num_divs>\n"
        "        divisions along each axis.\n",
        "prog"_a = ::prog_name
    );
}

auto main(int argc, char* argv[]) -> int {
    if (argc < 2) {
        pretty_print_err("no option provided.\n");
        return EXIT_FAILURE;
    }

    auto const input_param = std::string_view{argv[1]};
    auto const maybe_option = ::options.find(input_param);
    if (maybe_option == ::options.end()) {
        pretty_print_err("unrecognized option '{}'.\n", input_param);
        return EXIT_FAILURE;
    }

    errno = 0;
    try {
        switch (int const num_args = argc - 2; maybe_option->second) {
            using enum cli_option;
            case help: {
                display_help();
                return EXIT_SUCCESS;
            }
            case sphere: {
                check_num_args(4, num_args);
                u32 const radius = parse_u32(argv[2]);
                u32 const num_slices = parse_u32(argv[3]);
                u32 const num_stacks = parse_u32(argv[4]);
                auto output_file = open_output_file(argv[5]);
                generate_sphere(radius, num_slices, num_stacks, output_file);
                break;
            }
            case box: {
                check_num_args(3, num_args);
                u32 const num_units = parse_u32(argv[2]);
                u32 const grid_len = parse_u32(argv[3]);
                auto output_file = open_output_file(argv[4]);
                generate_box(num_units, grid_len, output_file);
                break;
            }
            case cone: {
                check_num_args(5, num_args);
                u32 const radius = parse_u32(argv[2]);
                u32 const height = parse_u32(argv[3]);
                u32 const num_slices = parse_u32(argv[4]);
                u32 const num_stacks = parse_u32(argv[5]);
                auto output_file = open_output_file(argv[6]);
                generate_cone(
                    radius, height, num_slices, num_stacks, output_file
                );
                break;
            }
            case plane: {
                check_num_args(3, num_args);
                u32 const len = parse_u32(argv[2]);
                u32 const num_divs = parse_u32(argv[3]);
                auto output_file = open_output_file(argv[4]);
                generate_plane(len, num_divs, output_file);
                break;
            }
        }
    } catch (std::exception const& e) {
        int const local_errno = errno;
        using namespace fmt::literals;
        pretty_print_err(
            "failed {primitive} generation with error '{err}'",
            "primitive"_a = input_param,
            "err"_a = e.what()
        );
        if (local_errno != 0) {
            fmt::print(stderr, ": '{}'", std::strerror(local_errno));
        }
        fmt::print(stderr, ".\n");
        return EXIT_FAILURE;
    }
}
