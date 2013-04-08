#pragma once

#include "graphics/OBJ.hpp"
#include "graphics/RenderComponent.hpp"

namespace game {

struct MapObjectComponent;

struct RenderMapObjectComponent : public RenderComponent {
    RenderMapObjectComponent(MapObjectComponent const*, TextureManager&);
    
    // Implement RenderComponent
    void render() override;

private:
    MapObjectComponent const* mapObject;
    OBJ obj;
};

} // namespace game
