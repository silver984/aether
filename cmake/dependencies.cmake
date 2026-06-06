include(FetchContent)

function(aether_fetch_dependencies TARGET)
    FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 6.0 GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(raylib)
    target_compile_definitions(raylib
        PRIVATE
            SUPPORT_FILEFORMAT_JPG=1 SUPPORT_MODULE_RAUDIO=0 SUPPORT_TRACELOG=0 SUPPORT_SCREEN_CAPTURE=0
            SUPPORT_CAMERA_SYSTEM=0 SUPPORT_GESTURES_SYSTEM=0 UPPORT_RPRAND_GENERATOR=0 SUPPORT_WINMM_HIGHRES_TIMER=0
            SUPPORT_CLIPBOARD_IMAGE=0 SUPPORT_FILEFORMAT_OBJ=0 SUPPORT_FILEFORMAT_MTL=0 SUPPORT_FILEFORMAT_IQM=0
            SUPPORT_FILEFORMAT_GLTF=0 SUPPORT_FILEFORMAT_VOX=0 SUPPORT_FILEFORMAT_M3D=0 SUPPORT_MESH_GENERATION=0
    )

    FetchContent_Declare(lua GIT_REPOSITORY https://github.com/lua/lua.git GIT_TAG v5.4.8 GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(lua)
    file(GLOB LUA_SOURCES "${lua_SOURCE_DIR}/*.c")
    list(FILTER LUA_SOURCES EXCLUDE REGEX "lua.c|luac.c")
    add_library(lua STATIC ${LUA_SOURCES})
    target_include_directories(lua PUBLIC ${lua_SOURCE_DIR})

    set(SOL2_BUILD_LUA OFF)
    set(SOL2_LUA_VERSION "5.4.8")
    FetchContent_Declare(sol2 GIT_REPOSITORY https://github.com/ThePhD/sol2.git GIT_TAG v3.5.0 GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(sol2)

    FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 12.1.0 GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(fmt)
    
    target_link_libraries(${TARGET}
        PRIVATE
            raylib lua sol2::sol2 fmt::fmt
    )
endfunction()