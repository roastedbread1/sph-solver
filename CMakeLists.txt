cmake_minimum_required(VERSION 3.12)

project(3Drenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "D:/dev/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glslang CONFIG REQUIRED)
find_package(volk CONFIG REQUIRED)
find_package(meshoptimizer CONFIG REQUIRED)
find_package(gli CONFIG REQUIRED)
find_package(rapidjson CONFIG REQUIRED)
find_package(taskflow CONFIG REQUIRED)
find_package(bullet CONFIG REQUIRED)
find_package(ImGuizmo CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(3Drenderer "")

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "D:/dev/vcpkg/vcpkg/installed/x64-windows/include")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
${CMAKE_SOURCE_DIR}/src
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 

rapidjson
assimp::assimp
${BULLET_LIBRARIES}
glfw
glm::glm
gli
glslang::glslang glslang::glslang-default-resource-limits glslang::SPIRV glslang::SPVRemapper
imguizmo::imguizmo
meshoptimizer::meshoptimizer
Taskflow::Taskflow
volk::volk volk::volk_headers
glad::glad
)