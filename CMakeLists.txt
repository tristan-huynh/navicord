cmake_minimum_required(VERSION 3.10)
project(navicord VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gather your main application sources
file(GLOB MAIN_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Gather the Discord SDK source files
file(GLOB DISCORD_SOURCES ${PROJECT_SOURCE_DIR}/discord/source/*.cpp)

# Build the discord SDK as a static library
add_library(discord_sdk ${DISCORD_SOURCES})
target_include_directories(discord_sdk PUBLIC ${PROJECT_SOURCE_DIR}/discord/header)

# Link the precompiled Discord SDK library (adjust the path and name if needed)
target_link_libraries(discord_sdk PRIVATE ${PROJECT_SOURCE_DIR}/discord/lib/discord_game_sdk.dll.lib)

# Build your main executable and link the SDK
add_executable(navicord ${MAIN_SOURCES})
target_link_libraries(navicord PRIVATE discord_sdk)