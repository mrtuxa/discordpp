# minimum CMake version required
cmake_minimum_required(VERSION 3.15)
# Project name, version and description
project(discord-bot VERSION 1.0 DESCRIPTION "A discord bot")

# Add DPP as dependency
add_subdirectory(libs/DPP)
# You can also add any other libs you want to use

# Create an executable
add_executable(${PROJECT_NAME}
        src/main.cpp
        # your other files...
        src/include/secrets.hpp
        src/commands/ping.hpp
        src/include/interaction_command.hpp
        src/commands/info.hpp
        src/include/uwufy.hpp
)

# Linking libraries
target_link_libraries(${PROJECT_NAME}
        dpp
)

# Specify includes
target_include_directories(${PROJECT_NAME} PRIVATE
        libs/DPP/include
)

# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
)