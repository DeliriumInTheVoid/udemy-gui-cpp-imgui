cmake_minimum_required(VERSION 3.22)

project(
    "UdemyImGui"
    VERSION 1.0.0
    LANGUAGES C CXX)

# Use Clang as the compiler
# set(CMAKE_C_COMPILER /usr/bin/clang)
# set(CMAKE_CXX_COMPILER /usr/bin/clang++)

# it's generates compile_commands.json for clangd language server
# https://clangd.llvm.org/installation#compile_commandsjson
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_GENERATOR "NMake Makefiles")

# set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/01_FileExplorer")
# add_subdirectory("01_FileExplorer")

# set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/02_Plotter")
# add_subdirectory("02_Plotter")

# set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/03_TextEditor")
# add_subdirectory("03_TextEditor")

# set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/04_DiffViewer")
# add_subdirectory("04_DiffViewer")

# set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/05_Paint")
# add_subdirectory("05_Paint")

# set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/06_Calendar")
# add_subdirectory("06_Calendar")

set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/07_CsvTool")
add_subdirectory("07_CsvTool")
