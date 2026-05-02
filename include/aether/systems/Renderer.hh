#pragma once

namespace ae {

class Funkin;

class Renderer final {
    friend class Funkin;
private:
    Renderer();
    ~Renderer();

public:
    Renderer(Renderer const&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator =(Renderer const&) = delete;
    Renderer& operator =(Renderer&&) = delete;

private:
    void start_draw() const;
    void end_draw() const;
};

}