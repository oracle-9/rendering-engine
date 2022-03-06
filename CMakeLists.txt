################################################################################
# Header                                                                       #
################################################################################
cmake_minimum_required(VERSION 3.16) # 3.16 required for Link-Time Optimization#
project("CG-2021-22" VERSION 1.0 LANGUAGES CXX)                                #
################################################################################


################################################################################
# Options                                                                      #
################################################################################
set(CMAKE_CXX_STANDARD 20) # C++20                                             #
set(CMAKE_CXX_STANDARD_REQUIRED True)                                          #
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json for IntelliSense.#
################################################################################


################################################################################
# Include Path                                                                 #
################################################################################
set(INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")                              #
################################################################################


################################################################################
# Libraries Path                                                               #
################################################################################
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")                                       #
################################################################################


################################################################################
# Targets                                                                      #
################################################################################
# Generator                                                                    #
list(                                                                          #
    APPEND GENERATOR_HEADERS                                                   #
    "${INCLUDE_PATH}/generator/util/pretty_print.hpp"                          #
    "${INCLUDE_PATH}/generator/primitives/module.hpp"                          #
    "${INCLUDE_PATH}/generator/primitives/box.hpp"                             #
    "${INCLUDE_PATH}/generator/primitives/cone.hpp"                            #
    "${INCLUDE_PATH}/generator/primitives/plane.hpp"                           #
    "${INCLUDE_PATH}/generator/primitives/sphere.hpp"                          #
)                                                                              #
                                                                               #
list(                                                                          #
    APPEND GENERATOR_SOURCES                                                   #
    "src/generator/main.cpp"                                                   #
    "src/generator/primitives/box.cpp"                                         #
    "src/generator/primitives/cone.cpp"                                        #
    "src/generator/primitives/plane.cpp"                                       #
    "src/generator/primitives/sphere.cpp"                                      #
)                                                                              #
                                                                               #
add_executable(generator ${GENERATOR_HEADERS} ${GENERATOR_SOURCES})            #
                                                                               #
target_include_directories(generator PRIVATE ${INCLUDE_PATH})                  #
                                                                               #
                                                                               #
# Engine                                                                       #
list(                                                                          #
    APPEND ENGINE_HEADERS                                                      #
)                                                                              #
                                                                               #
list(                                                                          #
    APPEND ENGINE_SOURCES                                                      #
    "src/engine/main.cpp"                                                      #
)                                                                              #
                                                                               #
add_executable(engine ${ENGINE_HEADERS} ${ENGINE_SOURCES})                     #
                                                                               #
target_include_directories(engine PRIVATE ${INCLUDE_PATH})                     #
################################################################################


################################################################################
# 3rd Party Libraries                                                          #
################################################################################
# brief_int                                                                    #
set(BRIEF_INT_DIR "${LIB_DIR}/brief_int")                                      #
                                                                               #
# {fmt}                                                                        #
set(FMT_DIR "${LIB_DIR}/fmt")                                                  #
add_subdirectory(${FMT_DIR})                                                   #
                                                                               #
# rapidxml                                                                     #
set(RAPIDXML_DIR "${LIB_DIR}/rapidxml")                                        #
                                                                               #
                                                                               #
# Generator include directories                                                #
target_include_directories(                                                    #
    generator                                                                  #
    PRIVATE SYSTEM ${FMT_DIR} ${BRIEF_INT_DIR}                                 #
)                                                                              #
                                                                               #
# Generator linked libraries                                                   #
target_link_libraries(generator fmt::fmt)                                      #
                                                                               #
                                                                               #
# Engine include directories                                                   #
target_include_directories(engine SYSTEM PRIVATE ${RAPIDXML_DIR})              #
                                                                               #
# Engine linked libraries                                                      #
target_link_libraries(engine fmt::fmt)                                         #
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
                                                                               #
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
                                                                               #
check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT USE_LTO_ERROR)                 #
                                                                               #
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
