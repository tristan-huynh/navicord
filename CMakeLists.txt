cmake_minimum_required(VERSION 3.10)
project(navicord VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Gather main application sources
file(GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Gather the Discord SDK source files and build them into a static library
file(GLOB DISCORD_SOURCES "${PROJECT_SOURCE_DIR}/lib/discord_game_sdk/source/*.cpp")

add_library(discord_sdk STATIC ${DISCORD_SOURCES})


target_include_directories(discord_sdk PUBLIC "${PROJECT_SOURCE_DIR}/lib/discord_game_sdk/include")





# Link the precompiled Discord SDK library to the discord_sdk target
if(WIN32)
    target_link_libraries(discord_sdk PRIVATE "${PROJECT_SOURCE_DIR}/lib/discord_game_sdk/lib/discord_game_sdk.dll.lib")
elseif(APPLE)
    target_link_libraries(discord_sdk PRIVATE "${PROJECT_SOURCE_DIR}/lib/discord_game_sdk/lib/discord_game_sdk.dylib")
else() # Linux
    target_link_libraries(discord_sdk PRIVATE "${PROJECT_SOURCE_DIR}/lib/discord_game_sdk/lib/discord_game_sdk.so")
endif()

# Build your main executable and link it with the discord_sdk library
add_executable(navicord src/main.cpp)

# Set the runtime search path (RPATH) for Linux and macOS
if(UNIX)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    if(APPLE)
        set(CMAKE_INSTALL_RPATH "@executable_path")
    endif()
endif()

# Define the shared Discord library path based on the platform
if(WIN32)
    set(DISCORD_SHARED_LIB "${PROJECT_SOURCE_DIR}/lib/discord_game_sdk/lib/discord_game_sdk.dll")
elseif(APPLE)
    set(DISCORD_SHARED_LIB "${PROJECT_SOURCE_DIR}/lib/discord_game_sdk/lib/discord_game_sdk.dylib")
else() # Linux
    set(DISCORD_SHARED_LIB "${PROJECT_SOURCE_DIR}/lib/discord_game_sdk/lib/discord_game_sdk.so")
endif()

# Add a post-build command to copy the shared library to the executable's directory
add_custom_command(TARGET navicord POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${DISCORD_SHARED_LIB}"
    $<TARGET_FILE_DIR:navicord>
)


