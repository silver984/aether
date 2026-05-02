target_sources(${aether}
    PRIVATE
        main.cc
        Funkin.cc
)

add_subdirectory(common)
add_subdirectory(systems)
add_subdirectory(internal)