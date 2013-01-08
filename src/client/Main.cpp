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
#include "world/MessageTypes.hpp"

#include "net/Message.hpp"
#include "net/MessageHub.hpp"
#include "net/MessageTypes.hpp"
#include "net/NetSystem.hpp"
#include "net/Definitions.hpp"
#include "net/NetStateStore.hpp"

using namespace game;
using namespace std::placeholders;
using std::bind;

ComponentList makeTeapot(NetEntityId id, ClientId owner, vec3 position) {
    auto physics = new PhysicsComponent(position);
    return {
        physics,
        new RenderCube(physics, vec3(1, 0, 0)),
        new NetComponent(0, id, owner, { new PhysicsNetState(physics) })
    };
}

ComponentList makePlayer(NetEntityId id, ClientId owner, vec3 position) {
    static vec3 colors[] = {
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 0, 1),
        vec3(1, 0, 1),
        vec3(0, 1, 1),
        vec3(1, 1, 0),
        vec3(0, 0, 0)
    };

    // Later: If I'm the owner, add input component or something.

    auto physics = new PhysicsComponent(position);
    return {
        physics,
        new RenderCube(physics, colors[owner]),
        new NetComponent(1, id, owner, { new PhysicsNetState(physics) })
    };
}

#define INTERPOLATION_FREQUENCY 100

// I'll split Client and Server up as soon as I'll find out what they do.
struct Client {
    sf::Clock clock; // tmp

    sf::Window& window;
    InputSource& input;
    Tasks tasks;

    TextureManager textures;
    ProgramManager programs;
    RenderSystem renderSystem;
    TickSystem tickSystem;
    NetSystem netSystem;
    EntityRegistry entities;

    PlayerInputSource playerInputSource;
    Entity* playerEntity;

    ENetHost* host;
    ENetPeer* peer;
    std::unique_ptr<MessageHub> messageHub;
    ClientId myId;

    Tick tick;
    bool startNextTick;
    bool isFirstTick;
    NetStateStore curState;
    NetStateStore nextState;
    float tickInterpolation;
    std::queue<NetStateStore> tickStateQueue;

    PlayerInput playerInput;

    Client(sf::Window& window, InputSource& input)
        : window(window),
          input(input),
          renderSystem(window, textures, programs),
          entities({ &renderSystem, &tickSystem, &netSystem }),
          playerInputSource(window, input),
          playerEntity(nullptr),
          host(nullptr),
          peer(nullptr),
          messageHub(makeMessageHub()),
          myId(0),
          tick(0),
          startNextTick(false),
          isFirstTick(true) {
        tasks.add(TICK_FREQUENCY, [&] () { startTick(); });
        tasks.add(TICK_FREQUENCY, [&] () { 
            if (peer)
                messageHub->send<PlayerInputMessage>(peer, playerInput);
        });
        tasks.add(INTERPOLATION_FREQUENCY, [&] () { interpolate(); });

        netSystem.registerType(0, makeTeapot);
        netSystem.registerType(1, makePlayer);

        messageHub->onMessage<CreateEntityMessage>([&]
        (NetEntityTypeId type, NetEntityId id, ClientId owner, vec3 pos) {
            auto entity = netSystem.createEntity(type, id, owner, pos);      

            ASSERT_MSG(myId > 0, "Should have received LoginMessage "
                                 << "before CreateEntityMessage.");
            if (owner == myId)
                playerEntity = entity; 
        });

        messageHub->onMessage<LoginMessage>([&]
        (ClientId id) {
            ASSERT(id > 0);
            myId = id;
        });

        playerInputSource.onPlayerInput.connect([&]
        (PlayerInput const& input) {
            playerInput = input;
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

                    ASSERT(tick > 0);

                    std::cout << "@" << clock.getElapsedTime().asMilliseconds()
                              << ": received state for tick " << tick
                              << std::endl;

                    tickStateQueue.emplace(tick);
                    netSystem.readRawStates(stream, tickStateQueue.back());

                    if (startNextTick)
                        startTick();
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
        if (tickStateQueue.size() < 1) {
            // Start the tick as soon as it arrives, so we don't have to wait
            // for Tasks to call startTick() again
            startNextTick = true;
            return;
        }

        if (isFirstTick) {
            if (tickStateQueue.size() < 2) {
                startNextTick = true;
                return;
            }

            curState = tickStateQueue.front();
            tickStateQueue.pop();

            nextState = tickStateQueue.front();
            tickStateQueue.pop();

            isFirstTick = false;
        } else {
            curState = nextState;

            nextState = tickStateQueue.front();
            tickStateQueue.pop();
        }

        tick = curState.tick();
        tickInterpolation = 0;

        netSystem.applyStates(curState);

        std::cout << "@" << clock.getElapsedTime().asMilliseconds()
                  << ": started tick " << curState.tick() << std::endl;
    }

    void interpolate() {
        if (tick == 0)
            return;

        if (tickInterpolation >= 1) {
            tickInterpolation = 1;
            startNextTick = true;
            std::cout << "@" << clock.getElapsedTime().asMilliseconds()
                      << ": end of tick" << std::endl;
            startTick();
            std::cout << "end interpolation start" << std::endl;
        }
        netSystem.interpolateStates(curState, nextState, tickInterpolation);

        tickInterpolation += (float)TICK_FREQUENCY / INTERPOLATION_FREQUENCY;
    }

    void update(Time delta) {
        receive();
        tasks.run(delta);
    }

    void render() {
        // For debugging
        if (playerEntity) {
            auto playerPhys = playerEntity->component<PhysicsComponent>();

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
    sf::Window window(sf::VideoMode(800, 600), "OpenGL",
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
