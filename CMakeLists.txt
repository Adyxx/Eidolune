cmake_minimum_required(VERSION 3.20)
project(Eidolune LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files
file(GLOB_RECURSE EIDOLUNE_SOURCES
    CONFIGURE_DEPENDS
    "Source/Eidolune/Core/*.cpp"
    "Source/Eidolune/Definitions/*.cpp"
    "Source/Eidolune/Engine/*.cpp"
    "Source/Eidolune/Loaders/*.cpp"
    "Source/Eidolune/Registry/*.cpp"
    "Source/Eidolune/Main/main.cpp"
)

# Add executable
add_executable(MainExe ${EIDOLUNE_SOURCES})

target_link_libraries(MainExe PRIVATE nlohmann_json::nlohmann_json)

# Include directories
target_include_directories(MainExe PRIVATE
    Source/Eidolune/Core
    Source/Eidolune/Definitions
    Source/Eidolune/Engine
    Source/Eidolune/External
    Source/Eidolune/Loaders
    Source/Eidolune/Main
    Source/Eidolune/Registry
)

# Enable debug symbols
set(CMAKE_BUILD_TYPE Debug)
