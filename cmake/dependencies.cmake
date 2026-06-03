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
    fetch(raylib https://github.com/raysan5/raylib.git 6.0)
    fetch(fmt https://github.com/fmtlib/fmt.git 12.1.0)

    target_compile_definitions(raylib
        PRIVATE
            SUPPORT_FILEFORMAT_JPG=1
            SUPPORT_MODULE_RAUDIO=0
            SUPPORT_TRACELOG=0
            SUPPORT_SCREEN_CAPTURE=0
            SUPPORT_CAMERA_SYSTEM=0
            SUPPORT_GESTURES_SYSTEM=0
            SUPPORT_RPRAND_GENERATOR=0
            SUPPORT_WINMM_HIGHRES_TIMER=0
            SUPPORT_CLIPBOARD_IMAGE=0
            SUPPORT_FILEFORMAT_OBJ=0
            SUPPORT_FILEFORMAT_MTL=0
            SUPPORT_FILEFORMAT_IQM=0
            SUPPORT_FILEFORMAT_GLTF=0
            SUPPORT_FILEFORMAT_VOX=0
            SUPPORT_FILEFORMAT_M3D=0
            SUPPORT_MESH_GENERATION=0
    )
    
    target_link_libraries(${target}
        PRIVATE
            raylib
            fmt::fmt
    )
endfunction()