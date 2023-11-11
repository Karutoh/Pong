cmake_minimum_required(VERSION 3.18.4)
project(TechDemo C CXX)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("Building for Windows.")
    set(IS_OS_WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("Building for Linux.")
    set(IS_OS_LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message("Building for Mac.")
    set(IS_OS_MAC TRUE)
endif ()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    message("Building for AMD64 architecture.")
    set(IS_ARCH_AMD64 TRUE)
elseif ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
    message("Building for ARM64 architecture.")
    set(IS_ARCH_ARM64 TRUE)
endif ()

set(CMAKE_CXX_STANDARD 20)

add_executable(Pong
        Levels/Game.h
        Levels/Game.cpp
        Main.cpp
)

if (IS_OS_WINDOWS)
    add_compile_definitions(VK_USE_PLATFORM_WIN32_KHR)
elseif (IS_OS_LINUX)
    add_compile_definitions(VK_USE_PLATFORM_XCB_KHR)
endif()

if (IS_ARCH_AMD64)
    target_link_directories(Pong PRIVATE "${PROJECT_SOURCE_DIR}/../Libraries/x64")

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_directories(Pong PRIVATE "${PROJECT_SOURCE_DIR}/../LWE/cmake-build-release")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_directories(Pong PRIVATE "${PROJECT_SOURCE_DIR}/../LWE/cmake-build-debug")
    elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        target_link_directories(Pong PRIVATE "${PROJECT_SOURCE_DIR}/../LWE/cmake-build-relwithdebinfo")
    endif()
elseif (IS_ARCH_ARM64)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_directories(Pong PRIVATE "${PROJECT_SOURCE_DIR}/../LWE/cmake-build-release-remote")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_directories(Pong PRIVATE "${PROJECT_SOURCE_DIR}/../LWE/cmake-build-debug-remote")
    endif()
endif ()

#target_include_directories(Pong PRIVATE "${PROJECT_SOURCE_DIR}/..")

if (IS_OS_WINDOWS)
    find_package(Vulkan REQUIRED)

    target_link_libraries(Pong PRIVATE Vulkan::Headers Vulkan::Vulkan ws2_32 avrt zlibstatic LWE)
else (IS_OS_LINUX)
    if (IS_ARCH_AMD64)
        find_package(Vulkan REQUIRED)

        target_link_libraries(Pong PRIVATE Vulkan::Headers Vulkan::Vulkan xcb xcb-xfixes xcb-xinput wooting_analog_wrapper LWE z asound)
    elseif (IS_ARCH_ARM64)
        target_compile_options(Pong PRIVATE -pthread)
        target_link_options(Pong PRIVATE -pthread)

        target_link_directories(Pong PRIVATE "$ENV{VULKAN_SDK}/lib")
        target_include_directories(Pong PRIVATE "$ENV{VULKAN_SDK}/include")

        target_link_libraries(Pong PRIVATE LWE vulkan xcb xcb-xfixes xcb-xinput z asound)
    endif ()
endif ()

#kernel32.lib user32.lib Gdi32.lib uuid.lib Ole32.lib