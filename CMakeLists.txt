cmake_minimum_required(VERSION 3.15)
project(cpu_sim)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# 1. Fetch Dependencies
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4)
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.6-docking)

# Disable GLFW extras to speed up build
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw imgui)

# 2. Build ImGui Library
# Using 'file(GLOB ...)' automatically picks up the .cpp files in the directory
file(GLOB IMGUI_SOURCES
    "${imgui_SOURCE_DIR}/*.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui
    PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui
    PUBLIC glfw
)

# 3. Find System OpenGL
find_package(OpenGL REQUIRED)

# 4. Main Application
add_executable(cpu_sim
    main.cpp
    cpu.cpp
    UI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/FCFS/FCFS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/Priority/Priority.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/RR/RR.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/SJF/SJF.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/SRTF/SRTF.cpp
)
target_include_directories(cpu_sim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms
)
target_link_libraries(cpu_sim PRIVATE imgui OpenGL::GL)

# System specific linking (Linux math/threads, macOS frameworks)
if(UNIX AND NOT APPLE)
    target_link_libraries(cpu_sim
        PRIVATE m dl pthread)
elseif(APPLE)
    target_link_libraries(cpu_sim
        PRIVATE "-framework Cocoa -framework IOKit -framework CoreVideo")
endif()
