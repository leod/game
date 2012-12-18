#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "core/EntityRegistry.hpp"
#include "core/Time.hpp"
#include "core/Tasks.hpp"
#include "input/SFMLInputSource.hpp"
#include "input/ClockTimeSource.hpp"
#include "graphics/RenderSystem.hpp"

using namespace game;

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(false);

    SFMLInputSource input;
    RenderSystem render;
    EntityRegistry entities(SystemList {
        &render     
    });
    Tasks tasks;

    bool running = true;

    tasks.add(60, [&] () { input.dispatch(); });

    input.onKeyPressed.connect([&] (KeyInput input) {
        if (input.code == Key::Escape)
            running = false; 
    });

    ClockTimeSource time;
    Time deltaTime;

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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        entities.withFamily(&RenderSystem::render, &render);
        window.display();

        deltaTime = time.nextDelta();
    }

    return 0;
}
