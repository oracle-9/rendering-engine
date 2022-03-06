################################################################################
# Header                                                                       #
################################################################################
cmake_minimum_required(VERSION 3.16)                                           #
project(CG-2021-22 VERSION 1.0)                                                #
################################################################################


################################################################################
# Options                                                                      #
################################################################################
set(CMAKE_CXX_STANDARD 20) # C++20                                             #
set(CMAKE_CXX_STANDARD_REQUIRED True)                                          #
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json for IntelliSense.#
################################################################################


################################################################################
# Targets                                                                      #
################################################################################
add_executable(generator "src/generator/main.cpp") # generator                 #
add_executable(engine "src/engine/main.cpp")       # engine                    #
################################################################################


################################################################################
# 3rd Party Libraries                                                          #
################################################################################
add_subdirectory("lib/brief_int")                                              #
add_subdirectory("lib/fmt")                                                    #
list(                                                                          #
    APPEND LIBS                                                                #
    brief_int                                                                  #
    fmt::fmt                                                                   #
)                                                                              #
foreach(TARGET generator engine)                                               #
    target_link_libraries(${TARGET} PUBLIC ${LIBS})                            #
endforeach()                                                                   #
################################################################################


################################################################################
# Compiler flags                                                               #
################################################################################
if(MSVC)                                                                       #
    # Compiling with Microsoft Visual C++.                                     #
    list(APPEND WARNING_FLAGS "/W4")                                           #
else()                                                                         #
    # Compiling with g++/clang++/etc.                                          #
    list(                                                                      #
        APPEND WARNING_FLAGS                                                   #
        "-Wall" "-Wextra" "-Wdouble-promotion"                                 #
        "-Wconversion" "-Wsign-conversion"                                     #
    )                                                                          #
endif()                                                                        #
foreach(TARGET generator engine)                                               #
    target_compile_options(                                                    #
        ${TARGET}                                                              #
        PRIVATE ${WARNING_FLAGS}                                               #
    )                                                                          #
endforeach()                                                                   #
################################################################################


################################################################################
# Link-Time-Optimization                                                       #
################################################################################
include(CheckIPOSupported)                                                     #
check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT USE_LTO_ERROR)                 #
if(LTO_SUPPORTED)                                                              #
    message(STATUS "LTO enabled")                                              #
    foreach(TARGET generator engine)                                           #
        set_property(                                                          #
            TARGET ${TARGET}                                                   #
            PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE                         #
    )                                                                          #
    endforeach()                                                               #
else()                                                                         #
    message(STATUS "LTO not supported: <${USE_LTO_ERROR}>")                    #
endif()                                                                        #
################################################################################
