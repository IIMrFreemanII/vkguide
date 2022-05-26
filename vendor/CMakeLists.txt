add_library(tinyobjloader STATIC)

target_sources(tinyobjloader PRIVATE
               tinyobjloader/tiny_obj_loader.h
               tinyobjloader/tiny_obj_loader.cc
               )

target_include_directories(tinyobjloader PUBLIC tinyobjloader)