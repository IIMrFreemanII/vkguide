cmake_minimum_required(VERSION 3.22)
project(vk_tutorial)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES
    src/log.cpp
    src/log.h
    src/main.cpp src/vk-engine.cpp src/vk-engine.h src/vk-initializers.cpp src/vk-initializers.h src/vk-types.h)

# Vendors
set(VENDOR_DIR vendor)

##spdlog header only
set(SPDLOG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${VENDOR_DIR}/spdlog/include)
##

##glm header only
set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${VENDOR_DIR}/glm)
##

#vk-bootstrap
add_subdirectory(${VENDOR_DIR}/vk-bootstrap)
set(VK_BOOTSTRAP_LIB vk-bootstrap::vk-bootstrap)
#

##sdl
add_subdirectory(${VENDOR_DIR}/sdl)

set(SDL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${VENDOR_DIR}/sdl/include)
set(SDL_LIB SDL2)
##

##vulkan
set(VULKAN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${VENDOR_DIR}/vulkan/include)
set(VULKAN_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${VENDOR_DIR}/vulkan/lib)
set(VULKAN_LIB libvulkan.1.dylib libvulkan.1.3.211.dylib)
##

## vulkan memory allocator
add_subdirectory(${VENDOR_DIR}/vulkan-memory-allocator)
set(VMA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${VENDOR_DIR}/vulkan-memory-allocator/include)
set(VMA_LIB VulkanMemoryAllocator)
##

#

set(INCLUDE_DIR
    ${SPDLOG_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${VULKAN_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${VMA_INCLUDE_DIR})

set(LIB_DIR
    ${VULKAN_LIB_DIR})

set(LIB
    ${VULKAN_LIB}
    ${SDL_LIB}
    ${VK_BOOTSTRAP_LIB}
    ${VMA_LIB})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR})
target_link_libraries(${PROJECT_NAME} ${LIB})