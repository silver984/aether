target_sources(${aether}
    PRIVATE
        main.cc
        Funkin.cc
)

add_subdirectory(common)
add_subdirectory(graphics)
add_subdirectory(math)
add_subdirectory(objects)
add_subdirectory(platforms)
add_subdirectory(states)
add_subdirectory(systems)