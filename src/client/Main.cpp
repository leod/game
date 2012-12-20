#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "core/Component.hpp"
#include "core/EntityRegistry.hpp"
#include "core/Time.hpp"
#include "core/Tasks.hpp"
#include "input/SFMLInputSource.hpp"
#include "input/ClockTimeSource.hpp"
#include "graphics/RenderSystem.hpp"
#include "graphics/RenderCube.hpp"
#include "physics/PhysicsComponent.hpp"
#include "world/PlayerInputSource.hpp"
#include "world/PlayerInputComponent.hpp"
#include "world/TickSystem.hpp"
#include "world/CircularMotion.hpp"

using namespace game;

ComponentList cube(vec3 position) {
    auto phys = new PhysicsComponent(position);
    return ComponentList {
        phys,
        new RenderCube(phys, vec3(1, 0, 0)),
        new CircularMotion(phys)
    };
}

ComponentList player(vec3 position, PlayerInputSource* input) {
    PhysicsComponent* physics = new PhysicsComponent(vec3());
    PlayerInputComponent* inputComponent = new PlayerInputComponent(input, physics);
    return ComponentList {
        physics,
        new RenderCube(physics, vec3(0, 0, 1)),
        inputComponent
    };
}

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(false);
    //window.setMouseCursorVisible(false);

    SFMLInputSource input;
    RenderSystem render(&window);
    TickSystem ticks;
    EntityRegistry entities(SystemList {
        &render,
        &ticks
    });
    Tasks tasks;

    bool running = true;

    tasks.add(60, [&] () { input.dispatch(); });
    tasks.add(30, [&] () { entities.withFamily(&TickSystem::tick, ticks); });

    input.onKeyPressed.connect([&] (KeyInput input) {
        if (input.code == Key::Escape)
            running = false; 
    });

    ClockTimeSource time;
    Time deltaTime;

    PlayerInputSource playerInput(&window, &input);
    Entity* playerEnt = entities.add(player(vec3(), &playerInput));
    entities.add(cube(vec3(2, 0, -15)));

    while (running) {
        {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    running = false;
                    continue;
                }

                input.onWindowEvent(event);
            }
        }

        tasks.run(deltaTime);

        // For debugging
        {
            auto playerPhys = playerEnt->component<PhysicsComponent>();

            vec3 cameraPosition = playerPhys->getPosition() + vec3(0, 10, -1);
            vec3 cameraTarget = playerPhys->getPosition();

            render.setCamera(cameraPosition, cameraTarget);
        }
        
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_QUADS);
        glColor3f(0.7, 0.7, 0.7);
        glVertex3f(-35, -1, -35);
        glColor3f(0.1, 0.1, 0.1);
        glVertex3f(-35, -1, 35);
        glColor3f(0.9, 0.9, 0.9);
        glVertex3f(35, -1, 35);
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(35, -1, -35);
        glEnd();
        entities.withFamily(&RenderSystem::render, render);
        
        glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(100.0, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 100.0);
        glEnd();

        window.display();

        deltaTime = time.nextDelta();
    }

    return 0;
}
