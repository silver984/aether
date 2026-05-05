#pragma once
#include <aether/math/size.hh>
#include <string_view>
#include <string>
#include <vector>
#include <cstdint>

namespace ae {

class Funkin;

class Window final {
    friend class Funkin;
public:
    Window();
    ~Window();
    Window(Window const&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] bool is_initialized() const;
    [[nodiscard]] bool should_close() const;
    [[nodiscard]] bool is_minimized() const;
    [[nodiscard]] bool was_resized() const;
    [[nodiscard]] std::string_view title() const;
    [[nodiscard]] size<int> screen_size() const;

private:
    bool init(std::string_view title, size<int> const& resolution, int target_fps);
    void shutdown();

    std::string title_;
    size<int> screen_size_;
    bool is_initialized_;
};

}