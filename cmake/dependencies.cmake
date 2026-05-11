include(FetchContent)

function(fetch dependency_name repo tag)
    FetchContent_Declare(${dependency_name}
        GIT_REPOSITORY ${repo}
        GIT_TAG ${tag}
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(${dependency_name})
endfunction()

function(aether_fetch_dependencies aether)
    fetch(raylib https://github.com/raysan5/raylib.git 6.0)
    fetch(fmt https://github.com/fmtlib/fmt.git 12.1.0)
    
    target_link_libraries(${aether}
        PRIVATE
            raylib
            fmt::fmt
    )
endfunction()