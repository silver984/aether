target_sources(${AETHER}
    PRIVATE
        Context.cc
        rl_converter.cc
        timer.cc
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${AETHER}
        PRIVATE
            log.cc
    )
endif()