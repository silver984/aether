target_sources(${aether}
    PRIVATE
        Context.cc
        rl_converter.cc
        timer.cc
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${aether}
        PRIVATE
            log.cc
    )
endif()