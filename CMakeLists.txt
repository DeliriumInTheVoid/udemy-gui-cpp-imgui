cmake_minimum_required(VERSION 3.22)

project(
    "UdemyImGui"
    VERSION 1.0.0
    LANGUAGES C CXX)

# it's generates compile_commands.json for clangd language server
# https://clangd.llvm.org/installation#compile_commandsjson
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_GENERATOR "NMake Makefiles")

set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/FileExplorer")
add_subdirectory("FileExplorer")
