#pragma once

#include <brief_int.hpp>
#include <fmt/os.h>
//#include <fstream>
//#include <iostream>

auto generate_box(
    brief_int::u32 comp,
    brief_int::u32 div,
    fmt::ostream& output_file
) -> void;

auto geraPlanoBox(
	char k,
    brief_int::u32 comp,
    brief_int::u32 div,
    brief_int::u32 v,
    fmt::ostream& output_file
) -> void;