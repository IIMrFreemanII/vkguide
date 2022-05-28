cmake_minimum_required(VERSION 3.22)
project(vkguide)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES
    src/log.cpp
    src/log.h
    src/main.cpp src/vk_engine.cpp src/vk_engine.h src/vk_initializers.cpp src/vk_initializers.h src/vk_types.h src/vk_mesh.cpp src/vk_mesh.h src/vk_textures.cpp src/vk_textures.h)

# Vendors
add_subdirectory(vendor)

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
    ${VMA_LIB}
    tinyobjloader
    stb_image)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR})
target_link_libraries(${PROJECT_NAME} ${LIB})