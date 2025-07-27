cmake_minimum_required(VERSION 3.20)

set(VCPKG_APPLOCAL_DEPS OFF)

project(Eidolune LANGUAGES CXX)

# Optional: Vcpkg toolchain file should be passed via CLI, not hardcoded here:
# -DCMAKE_TOOLCHAIN_FILE=C:/Users/Doma/vcpkg/scripts/buildsystems/vcpkg.cmake

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE EIDOLUNE_SOURCES
    CONFIGURE_DEPENDS
    "Source/Eidolune/Core/*.cpp"
    "Source/Eidolune/Definitions/*.cpp"
    "Source/Eidolune/Engine/*.cpp"
    "Source/Eidolune/Loaders/*.cpp"
    "Source/Eidolune/Registry/*.cpp"
    "Source/Eidolune/Classes/*.cpp"
    "Source/Eidolune/Main/main.cpp"
)


find_package(libpqxx CONFIG REQUIRED)

message(STATUS "Found libpqxx targets:")
get_target_property(_pqxx_inc_dirs libpqxx::pqxx INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "Include dirs for libpqxx::pqxx = ${_pqxx_inc_dirs}")

add_executable(MainExe ${EIDOLUNE_SOURCES})

target_compile_options(MainExe PRIVATE /GR)

target_link_libraries(MainExe
    PRIVATE
        nlohmann_json::nlohmann_json
        libpqxx::pqxx
)

target_include_directories(MainExe PRIVATE
    Source/Eidolune/Core
    Source/Eidolune/Definitions
    Source/Eidolune/Engine
    Source/Eidolune/External
    Source/Eidolune/Loaders
    Source/Eidolune/Main
    Source/Eidolune/Registry
    Source/Eidolune/Classes
    C:/Users/Doma/vcpkg/installed/x64-windows/include
)
