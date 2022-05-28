add_library(tinyobjloader STATIC)

target_sources(tinyobjloader PRIVATE
               tinyobjloader/tiny_obj_loader.h
               tinyobjloader/tiny_obj_loader.cc
               )

target_include_directories(tinyobjloader PUBLIC tinyobjloader)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb_image)