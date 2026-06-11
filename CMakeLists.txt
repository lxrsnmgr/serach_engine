cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # For clang-tidy.
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)              # Compile as C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Require C++17 support

project(simple_search_engine VERSION 0.1.0  LANGUAGES CXX)

include(CTest)
enable_testing()

######################################################################################################################
# COMPILER SETUP
######################################################################################################################
# Compiler flags.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fPIC")

set(GCC_COVERAGE_LINK_FLAGS    "-fPIC")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

set(SRC_CXX_FLAGS 
                "-std=c++17" 
                # "-Wall" 
                # "-Wextra"
                # "-Wpedantic"
                # "-Werror"
                # "-Wconversion"
                # "-Wfloat-equal"
                # "-Wformat-nonliteral"
                # "-Wformat-security"
                # "-Wformat-y2k"
                # "-Wformat=2"
                # "-Wimport"
                # "-Winvalid-pch"
                # "-Wmissing-declarations"
                # "-Wmissing-field-initializers"
                # "-Wmissing-format-attribute"
                # "-Wmissing-include-dirs"
                # "-Wmissing-noreturn"
                # "-Wpacked"
                # "-Wpointer-arith"
                # "-Wredundant-decls"
                # "-Wshadow"
                # "-Wstack-protector"
                # "-Wswitch-enum"
                # "-Wundef"
                # "-Wwrite-strings"
                )

# Includes.
set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
set(THIRD_PARTY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party)
include_directories(
                    ${SRC_INCLUDE_DIR} 
                    ${TEST_INCLUDE_DIR} 
                    ${THIRD_PARTY_INCLUDE_DIR}
                    )

# Iinks.
link_directories(${THIRD_PARTY_INCLUDE_DIR})

################################################################
include(build_support/third_party.cmake)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)

################################################################
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_subdirectory(src)
add_subdirectory(test)

################################################################
# "make docs" to build the doxygen documentation.
################################################################
include(build_support/use_doxygen_doc.cmake)

add_doxygen_doc(
  BUILD_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/_build
  DOXY_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
  TARGET_NAME
    docs
  COMMENT
    "HTML documentation"
  )
