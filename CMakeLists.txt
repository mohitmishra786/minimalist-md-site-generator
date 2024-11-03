cmake_minimum_required(VERSION 3.10)
project(markdown-site-gen C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find cmark package
find_package(PkgConfig REQUIRED)
pkg_check_modules(CMARK REQUIRED libcmark)

# Set source files
set(SOURCES
    src/main.c
    src/parser.c
    src/renderer.c
    src/utils.c
    src/config.c
)

# Create executable
add_executable(markdown-site-gen ${SOURCES})

# Include directories
target_include_directories(markdown-site-gen PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMARK_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(markdown-site-gen PRIVATE ${CMARK_LIBRARIES})

# Install
install(TARGETS markdown-site-gen
    RUNTIME DESTINATION bin
)