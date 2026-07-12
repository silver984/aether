include(cmake/CPM.cmake)

function(aether_fetch_dependencies TARGET)
    # raylib
    CPMAddPackage(
        NAME raylib
        GITHUB_REPOSITORY raysan5/raylib
        GIT_TAG 6.0
        GIT_SHALLOW TRUE
    )

    target_compile_definitions(raylib PRIVATE
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

    # lua
    CPMAddPackage(
        NAME lua-src
        GITHUB_REPOSITORY lua/lua
        GIT_TAG v5.4.8
        GIT_SHALLOW TRUE
    )

    file(GLOB LUA_SRC "${lua-src_SOURCE_DIR}/*.c")
    list(REMOVE_ITEM
        LUA_SRC
        "${lua-src_SOURCE_DIR}/lua.c"
        "${lua-src_SOURCE_DIR}/onelua.c"
    )

    add_library(lua STATIC ${LUA_SRC})
    target_include_directories(lua PUBLIC "${lua-src_SOURCE_DIR}")

    # sol2
    set(SOL2_BUILD_LUA OFF)
    set(SOL2_LUA_VERSION "5.4.8")

    CPMAddPackage(
        NAME sol2
        GITHUB_REPOSITORY ThePhD/sol2
        GIT_TAG v3.5.0
        GIT_SHALLOW TRUE
    )

    # fmt
    CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 12.1.0
        GIT_SHALLOW TRUE
    )

    target_link_libraries(${TARGET}
        PRIVATE
            raylib
            lua
            sol2::sol2
            fmt::fmt
    )
endfunction()