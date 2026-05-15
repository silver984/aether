target_sources(${AETHER}
    PRIVATE
        Aether.cc
        main.cc
)

add_subdirectory(common)
add_subdirectory(math)
add_subdirectory(objects)
add_subdirectory(platforms)
add_subdirectory(states)
add_subdirectory(systems)