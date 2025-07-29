cmake_minimum_required(VERSION 3.20)
project(Eidolune LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE EIDOLUNE_SOURCES
    CONFIGURE_DEPENDS
    "Source/Eidolune/Core/*.cpp"
    "Source/Eidolune/Definitions/*.cpp"
    "Source/Eidolune/Engine/*.cpp"
    "Source/Eidolune/Loaders/*.cpp"
    "Source/Eidolune/Registry/*.cpp"
    "Source/Eidolune/Classes/*.cpp"
    "Source/Eidolune/Gacha/*.cpp"
    "Source/Eidolune/Abilities/ClassAbilities/*.cpp"
    "Source/Eidolune/Abilities/Other/*.cpp"
    "Source/Eidolune/Main/main.cpp"
    "Source/Eidolune/Http/ApiClient.cpp"
)

add_executable(MainExe ${EIDOLUNE_SOURCES})

# Link to vcpkg-installed libraries
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(MainExe
    PRIVATE
        cpr::cpr
        nlohmann_json::nlohmann_json
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
    Source/Eidolune/Abilities
    Source/Eidolune/Gacha
    Source/Eidolune/Abilities/ClassAbilities
    Source/Eidolune/Abilities/Other
    Source/Eidolune/Http
)

# cmake -S . -B build -G "Visual Studio 17 2022" -T ClangCL,host=x86 -A win32 -DCMAKE_TOOLCHAIN_FILE=C:/Users/Doma/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x86-windows
