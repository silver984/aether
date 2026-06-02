target_sources(${AETHER}
    PRIVATE
        AudioManager.cc
        Context.cc
        SceneScheduler.cc
        Renderer.cc
        Window.cc
)

add_subdirectory(resource)