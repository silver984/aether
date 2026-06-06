target_sources(${AETHER}
    PRIVATE
        math/mat3.cc
        scenes/TestScene.cc
        Aether.cc
        Context.cc
        CoreServices.cc
        main.cc
        ResourceServices.cc
)

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${AETHER} PRIVATE platforms/win32.cc)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${AETHER} PRIVATE log.cc)
endif()

add_subdirectory(objects)
add_subdirectory(services)
add_subdirectory(util)