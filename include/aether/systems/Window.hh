#pragma once
#include <aether/math/size.hh>
#include <aether/common/Callback.hh>
#include <string_view>
#include <string>
#include <vector>

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
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] bool is_initialized() const;
    [[nodiscard]] bool should_close() const;
    [[nodiscard]] bool is_minimized() const;
    [[nodiscard]] std::string_view title() const;
    [[nodiscard]] size<int> screen_size() const;
    void on_resize(Callback&& fn);

private:
    bool init(std::string_view title, size<int> const& resolution, int target_fps);
    void shutdown();
    void update();

    std::string title_;
    size<int> screen_size_;
    std::vector<Callback> resize_callbacks_;
    bool is_initialized_;
};

}