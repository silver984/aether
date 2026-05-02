#pragma once
#include <aether/math/size.hh>
#include <string_view>
#include <string>

namespace ae {

class Funkin;

class Window final {
    friend class Funkin;
private:
    Window();
    ~Window();

public:
    Window(Window const&) = delete;
    Window(Window&&) = delete;
    Window& operator =(Window const&) = delete;
    Window& operator =(Window&&) = delete;

    [[nodiscard]] bool is_initialized() const;
    [[nodiscard]] bool should_close() const;
    [[nodiscard]] bool is_minimized() const;
    [[nodiscard]] std::string_view title() const;

private:
    bool init(std::string_view title, size<int> const& resolution, int target_fps);
    void shutdown();

    std::string title_;
    bool is_initialized_;
};

}