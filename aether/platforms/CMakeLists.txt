if(WIN32)
    target_sources(${aether}
        PRIVATE
            win32.cc
    )
endif()