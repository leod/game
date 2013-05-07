#include "world/Map.hpp"

#include <iostream>

#include <boost/none.hpp>

#include "math/Intersection.hpp"
#include "util/Print.hpp"

using boost::none;

namespace game {

static mat4 transform(vec3 const& position, vec3 const& scale,
                      float yRotation) {
    return
        glm::scale(glm::rotate(glm::translate(mat4(), position),
                               yRotation, vec3(0, 1, 0)),
                   scale);
}

static BoundingBox transformBoundingBox(mat4 const& t, BoundingBox const& b) {
    return {
        vec3(t * vec4(b.min, 1.0)),
        vec3(t * vec4(b.max, 1.0))
    };
}

MapObjectComponent::MapObjectComponent(MapObjectType const* type,
                                       vec3 const& position, vec3 const& scale,
                                       float yRotation)
    : type(type),
      position(position),
      modelTransform(transform(position, scale, yRotation)),
      bbox(transformBoundingBox(modelTransform, type->bbox)) {
}

Map::Map()
    : Map([] (MapObjectComponent const*) { return ComponentList(); }) {
}

Map::Map(Inject const& inject)
    : inject(inject),
      testType({ "box.obj", { vec3(-1, -1, -1), vec3(1, 1, 1) } }) {
}

void Map::createTestMap() {
    auto block = [&] (MapObjectType const* t, vec3 a, vec3 b, float c) {
        addObject(new MapObjectComponent(t, a, b, c));
    };

    block(&testType, vec3(-7, 0, 0), vec3(1, 2, 7), 0);
    block(&testType, vec3(7, 0, 0), vec3(1, 2, 7), 0);
    block(&testType, vec3(0, 0, -15), vec3(15, 1, 1), 0);
}

void Map::addObject(MapObjectComponent* object) {
    auto entities = getManager();
    ASSERT(entities);

    auto components = inject(object);
    components.push_back(object);
    entities->create(std::move(components));
}

Intersection rayMapIntersection(Ray const& ray, Map const& map,
        MapObjectComponent const** outObject, vec3* outNormal) {
    Intersection closest = boost::none;

    map.iterate([&] (MapObjectComponent const* object) {
        vec3 normal;
        auto intersection = rayBoundingBoxIntersection(ray, object->bbox,
                                                       &normal);
        if (intersection < closest) {
            closest = intersection;
            if (outObject) *outObject = object;
            if (outNormal) *outNormal = normal;
        }
    });

    return closest;
}

} // namespace game
