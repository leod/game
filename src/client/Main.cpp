#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include <SFML/Window.hpp>
#include <GL/glew.h>

#include "core/Component.hpp"
#include "core/EntityRegistry.hpp"
#include "core/Time.hpp"
#include "core/Tasks.hpp"
#include "input/SFMLInputSource.hpp"
#include "input/ClockTimeSource.hpp"

#include "opengl/TextureManager.hpp"
#include "opengl/ProgramManager.hpp"
#include "opengl/Error.hpp"
#include "graphics/RenderSystem.hpp"
#include "graphics/RenderCube.hpp"

#include "physics/PhysicsComponent.hpp"

#include "world/PlayerInputSource.hpp"
#include "world/PlayerInputComponent.hpp"
#include "world/TickSystem.hpp"
#include "world/CircularMotion.hpp"
#include "world/PhysicsNetState.hpp"

#include "net/Message.hpp"
#include "net/MessageHub.hpp"
#include "net/MessageTypes.hpp"
#include "net/NetSystem.hpp"
#include "net/Definitions.hpp"
#include "net/NetStateStore.hpp"

using namespace game;

ComponentList teapot(NetEntityId id, vec3 position) {
    auto physics = new PhysicsComponent(position);
    return {
        physics,
        new RenderCube(physics, vec3(1, 0, 0)),
        new NetComponent(0, id, { new PhysicsNetState(physics) })
    };
}

// I'll split Client and Server up as soon as I'll find out what they do.
struct Client {
    sf::Window& window;
    InputSource& input;
    Tasks tasks;

    TextureManager textures;
    ProgramManager programs;
    RenderSystem renderSystem;
    TickSystem tickSystem;
    NetSystem netSystem;
    EntityRegistry entities;

    PlayerInputSource playerInput;
    Entity* playerEnt;

    ENetHost* host;
    ENetPeer* peer;
    std::unique_ptr<MessageHub> messageHub;

    Tick tick;
    std::queue<NetStateStore> tickStateQueue;

    Client(sf::Window& window, InputSource& input)
        : window(window),
          input(input),
          renderSystem(window, textures, programs),
          entities({ &renderSystem, &tickSystem, &netSystem }),
          playerInput(window, input),
          playerEnt(nullptr),
          host(nullptr),
          peer(nullptr),
          messageHub(makeMessageHub()),
          tick(0) {
        tasks.add(TICK_FREQUENCY, [&] () { startTick(); });

        netSystem.registerType(0, teapot);

        messageHub->onMessage<CreateEntity>(
                [&] (NetEntityTypeId type,
                     NetEntityId id,
                     vec3 pos) -> void {
                    netSystem.createEntity(type, id, pos);      
                });
    }

    ~Client() {
        if (host)
            enet_host_destroy(host);
        if (peer)
            enet_peer_reset(peer);
    }

    void connect(ENetAddress const& address) {
        host = enet_host_create(nullptr, 1, 2, 0, 0);
        if (host == nullptr)
            throw std::runtime_error("Failed to create ENet client host");

        peer = enet_host_connect(host, &address, 2, 0);

        if (peer == nullptr)
            throw std::runtime_error("Failed to connect to host");

        // Wait for connection attempt to succeed
        auto const maxWaitTime = 5000; // Milliseconds

        ENetEvent event;
        if (enet_host_service(host, &event, maxWaitTime) <= 0 ||
            event.type != ENET_EVENT_TYPE_CONNECT)
            throw std::runtime_error("Failed to connect to host");
    }

    void receive() {
        ENetEvent event;

        while (enet_host_service(host, &event, 0) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                if (event.channelID == CHANNEL_MESSAGES)
                    messageHub->dispatch(event.peer, event.packet);
                else {
                    BitStreamReader stream(event.packet->data,
                                           event.packet->dataLength);
                    Tick tick;
                    read(stream, tick);

                    std::cout << "Received state for tick " << tick << std::endl;

                    NetStateStore store;
                    netSystem.readRawStates(stream, store);

                    tickStateQueue.push(store);
                }

                enet_packet_destroy(event.packet);

                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected" << std::endl;
                break;

            default:
                ASSERT(false);
            }
        }
    }

    void startTick() {
        if (tickStateQueue.empty())
            return;

        auto const& nextState = tickStateQueue.front();
        netSystem.applyStates(nextState);

        tickStateQueue.pop();
    }

    void update(Time delta) {
        receive();
        tasks.run(delta);
    }

    void render() {
        // For debugging
        if (playerEnt) {
            auto playerPhys = playerEnt->component<PhysicsComponent>();

            vec3 cameraPosition = playerPhys->getPosition() +
                                  vec3(0, 8, -0.001);
            vec3 cameraTarget = playerPhys->getPosition();

            renderSystem.setCamera(cameraPosition, cameraTarget);
        }
        else
            renderSystem.setCamera(vec3(0, 8, -0.001), vec3(0, 0, 0));
        
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.3, 0.3, 0.3, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(renderSystem.getView()));
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(renderSystem.getProjection()));

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

        /*glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(100.0, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 100.0);
        glEnd();*/

        renderSystem.render();

        window.display();
    }
};

int main()
{
    sf::Window window(sf::VideoMode(1280, 1024), "OpenGL",
            sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
    //window.setMouseCursorVisible(false);

    // Now that we have an OpenGL context, we can initialize GLEW
    {
        auto err = glewInit();
        if (err != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW: "
                      << glewGetErrorString(err) << "."
                      << std::endl;
            return 1;
        }
    }

    enet_initialize();

    SFMLInputSource input;
    Tasks tasks;

    bool running = true;

    tasks.add(60, [&] () { input.dispatch(); });

    input.onKeyPressed.connect([&] (KeyInput input) {
        if (input.code == Key::Escape)
            running = false; 
    });

    ClockTimeSource time;
    Time deltaTime;

    Client client(window, input);

    {
        ENetAddress address;
        enet_address_set_host(&address, "localhost");
        address.port = 20000;

        client.connect(address);
    }

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
        client.update(deltaTime);
        client.render();
        
        checkGLError();

        deltaTime = time.nextDelta();
    }

    return 0;
}
