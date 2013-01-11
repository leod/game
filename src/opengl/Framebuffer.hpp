#pragma once

#include <functional>
#include <memory>

#include <GL/glew.h>

#include "math/Math.hpp"
#include "opengl/Texture.hpp"

namespace game {

struct Framebuffer {
    enum Config {
        COLOR = 1,
        DEPTH = 2
    };

    enum Clear : bool {
        NO_CLEAR = false,
        CLEAR = true
    };

    Framebuffer(Config config, ivec2 size);
    ~Framebuffer();

    Texture const& getColorTexture() const;

    template<typename F>
    void renderInto(F f, Clear clear = NO_CLEAR) const {
        renderIntoImpl(f, clear); 
    }

private:
    Config config;
    ivec2 size;

    GLuint name;
    GLuint depthName;

    std::unique_ptr<Texture> colorTexture;

    void renderIntoImpl(std::function<void()> const&, Clear) const;
};

} // namespace game
