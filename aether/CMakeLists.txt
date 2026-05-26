target_sources(${AETHER}
    PRIVATE
        Aether.cc
        main.cc
)

add_subdirectory(math_types)
add_subdirectory(objects)
add_subdirectory(platforms)
add_subdirectory(states)
add_subdirectory(systems)
add_subdirectory(util)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(debug)
endif()