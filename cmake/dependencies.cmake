include(FetchContent)

function(fetch dependency_name repo tag)
    FetchContent_Declare(${dependency_name}
        GIT_REPOSITORY ${repo}
        GIT_TAG ${tag}
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(${dependency_name})
endfunction()

function(aether_fetch_dependencies target)
    add_compile_definitions(SUPPORT_FILEFORMAT_JPG=1) # enable for raylib
    add_compile_definitions(SUPPORT_MODULE_RAUDIO=0) # disable for raylib, use own miniaudio instead
    add_compile_definitions(SUPPORT_TRACELOG=0) # disable for raylib, use own logging instead
    add_compile_definitions(SUPPORT_SCREEN_CAPTURE=0) # disable for raylib, we don't want this
    fetch(raylib https://github.com/raysan5/raylib.git 6.0)
    fetch(fmt https://github.com/fmtlib/fmt.git 12.1.0)
    
    target_link_libraries(${target}
        PRIVATE
            raylib
            fmt::fmt
    )
endfunction()