#pragma once

#include <vector>

#include <boost/optional.hpp>

#include "core/Component.hpp"
#include "core/System.hpp"
#include "math/Math.hpp"
#include "math/Intersection.hpp"

namespace game {

struct MapObjectType {
    std::string modelFilename;

    BoundingBox bbox;
};

struct MapObjectComponent : public Component<MapObjectComponent> {
    MapObjectType const* type;
    vec3 const position;
    mat4 const modelTransform;
    BoundingBox const bbox;
    
    MapObjectComponent(MapObjectType const*, vec3 const& position,
                       vec3 const& scale, float yRotation);
};

struct Map : public System<MapObjectComponent> {
    typedef std::function<ComponentList(MapObjectComponent const*)> Inject;

    Map();
    Map(Inject const&);

    void createTestMap();

private:
    Inject const inject;

    MapObjectType testType;

    void addObject(MapObjectComponent*);
};

Intersection rayMapIntersection(Ray const&, Map const&,
        MapObjectComponent const** outObject = nullptr,
        vec3* outNormal = nullptr);

} // namespace game
