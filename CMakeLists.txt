cmake_minimum_required(VERSION 3.15)

# 1. Specify languages to help CMake optimize its checks
project(PocketTrack VERSION 1.0 LANGUAGES CXX)

# Enforce strict C++17 compliance
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disables compiler-specific extensions for better portability

# 2. Explicitly list source files (Avoid GLOB)
# Adjust the path of main.cpp if it is located inside the src/ folder
set(SOURCES
    src/main.cpp 
    src/Persistence.cpp
    src/Playlist.cpp
    src/SongLibrary.cpp
    src/Recommender.cpp
    src/Song.cpp
    src/Userprofile.cpp
    src/Player.cpp
)
# Create the executable
add_executable(PocketTrack ${SOURCES})

# 3. Target-specific includes (Avoid global include_directories)
target_include_directories(PocketTrack PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 4. Enable rigorous compiler warnings to catch bugs early
if(MSVC)
    # Warning flags for Visual Studio compiler
    target_compile_options(PocketTrack PRIVATE /W4)
else()
    # Warning flags for GCC/Clang (like MinGW)
    target_compile_options(PocketTrack PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 5. Copy data files reliably
# Using CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR is safer for complex builds
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/data/songs.csv 
    ${CMAKE_CURRENT_BINARY_DIR}/songs.csv 
    COPYONLY
)