target_sources(${AETHER}
    PRIVATE
        Aether.cc
        main.cc
)

add_subdirectory(math_types)
add_subdirectory(objects)
add_subdirectory(platforms)
add_subdirectory(states)
add_subdirectory(services)
add_subdirectory(util)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${AETHER}
        PRIVATE
            log.cc
    )
endif()