target_sources(${AETHER}
    PRIVATE
        math/mat3.cc
        scenes/test_scene.cc
        app.cc
        context.cc
        core_services.cc
        main.cc
        resource_services.cc
)

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${AETHER} PRIVATE platforms/win32.cc)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${AETHER} PRIVATE log.cc)
endif()

add_subdirectory(abstract)
add_subdirectory(lua)
add_subdirectory(objects)
add_subdirectory(services)
add_subdirectory(util)