if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${aether}
        PRIVATE
            win32.cc
    )
endif()