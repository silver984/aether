if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${AETHER}
        PRIVATE
            win32.cc
    )
endif()