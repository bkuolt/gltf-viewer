add_library(libgltf STATIC
    Loader.cpp
)

find_package(nlohmann_json REQUIRED)
target_include_directories(libgltf PRIVATE
    ${nlohmann_json_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS}
    ${TinyGLTF_INCLUDE_DIRS}
)

target_link_libraries(libgltf PRIVATE
    nlohmann_json::nlohmann_json
    #libpng::libpng
    #libjpeg::libjpeg
    #zlib::zlib
    #zstd::zstd
)
