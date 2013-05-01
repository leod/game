#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include "core/Component.hpp"
#include "core/EntityManager.hpp"
#include "core/Time.hpp"
#include "core/Tasks.hpp"
#include "core/Log.hpp"

#include "util/Profiling.hpp"

#include "input/SFMLInputSource.hpp"
#include "input/ClockTimeSource.hpp"

#include "world/PhysicsSystem.hpp"
#include "world/PhysicsComponent.hpp"

#include "world/Map.hpp"

#include "net/NetSystem.hpp"
#include "net/NetStateStore.hpp"
#include "net/ENetReceiver.hpp"
#include "net/EventQueue.hpp"

#include "world/TickSystem.hpp"
#include "world/PhysicsNetState.hpp"
#include "world/EventTypes.hpp"
#include "world/ProjectileComponent.hpp"
#include "world/PlayerComponent.hpp"

#include "opengl/TextureManager.hpp"
#include "opengl/ProgramManager.hpp"
#include "opengl/Error.hpp"

#include "graphics/GroundRenderer.hpp"
#include "graphics/RenderSystem.hpp"
#include "graphics/RenderCube.hpp"
#include "graphics/RenderMapObjectComponent.hpp"
#include "graphics/VisionSystem.hpp"
#include "graphics/RenderOBJComponent.hpp"
#include "graphics/RenderPlayerComponent.hpp"

#include "sound/SoundPlayer.hpp"

#include "client/PlayerInputComponent.hpp"
#include "client/PlayerInputSource.hpp"

using namespace game;

ComponentList makeTeapot(NetEntityId, ClientId);
ComponentList makePlayer(NetEntityId, ClientId);
ComponentList makeProjectile(NetEntityId, ClientId);

#define INTERPOLATION_FREQUENCY 100

struct Client;
Client* client = nullptr; // obvious hack

static ComponentList makeMapObject(MapObjectComponent const* object);

struct Client : public ENetReceiver {
    sf::Clock clock; // tmp

    sf::RenderWindow& window;
    InputSource& input;
    Tasks tasks;

    EventHub eventHub;

    EventQueue eventQueue;

    Map map;

    TextureManager textures;
    ProgramManager programs;

    PhysicsSystem physicsSystem;
    VisionSystem visionSystem;
    RenderSystem renderSystem;
    TickSystem tickSystem;
    NetSystem netSystem;
    EntityManager entities;

    PlayerInputSource playerInputSource;
    Entity* playerEntity;

    ENetPeer* peer;
    ClientId myId;

    Tick tick;
    bool startNextTick;
    bool isFirstTick;
    NetStateStore curState;
    NetStateStore nextState;
    float tickInterpolation;

    std::queue<NetStateStore> tickStateQueue;

    PlayerInput playerInput;

    sf::Font font;

    Time timeSentLastPing;
    bool haveUnrepliedPing;

    SoundPlayer soundPlayer;

    GroundRenderer groundRenderer;

    Client(sf::RenderWindow& window, InputSource& input)
        : ENetReceiver(),
          window(window),
          input(input),
          map(makeMapObject),
          physicsSystem(map),
          visionSystem(map, programs),
          renderSystem(window, textures, programs, visionSystem),
          entities({ &map,
                     &physicsSystem,
                     &renderSystem,
                     &visionSystem,
                     &tickSystem,
                     &netSystem }),
          playerInputSource(window, input),
          playerEntity(nullptr),
          peer(nullptr),
          myId(0),
          tick(0),
          startNextTick(false),
          isFirstTick(true),
          timeSentLastPing(clock.getElapsedTime()),
          haveUnrepliedPing(false),
          groundRenderer(map, textures, programs, visionSystem) {
        ::client = this; // obvious hack

        SoundPlayer::setGlobalVolume(100);

        map.createTestMap();

        tasks.add(TICK_FREQUENCY, [&] () { startTick(); });
        tasks.add(TICK_FREQUENCY, [&] () { 
            if (!playerEntity || !peer)
                return;

            sendEvent<PlayerInputWish>(peer, playerInput);
            enet_host_flush(host);

            // Prediction
#ifdef USE_PREDICTION
            auto input =
                playerEntity->component<PlayerInputComponent>();
            if (input) {
                input->onPlayerInput(playerInput);
            }

            if (playerInput.isWalking()) {
                auto physics = playerEntity->component<PhysicsComponent>();
                if (physics)
                    soundPlayer.play(Sound::FOOTSTEP, physics->getPosition());
            }
#endif
        });

        tasks.add(INTERPOLATION_FREQUENCY, [&] () { interpolate(); });

        eventHub.subscribe<CreateEntityOrder>(this,
                &Client::onCreateEntityOrder);
        eventHub.subscribe<RemoveEntityOrder>(this,
                &Client::onRemoveEntityOrder);
        eventHub.subscribe<PlayerPositionOrder>(this,
                &Client::onPlayerPositionOrder);
        eventHub.subscribe<Pong>(this, &Client::onPong);
        eventHub.subscribe<LoggedInOrder>(this, &Client::onLoggedInOrder);

        input.onKeyPressed.connect(this, &Client::onKeyPressed);

        netSystem.registerType(
                { 0, { &physicsNetStateType }, makeTeapot });
        netSystem.registerType(
                { 1, { &physicsNetStateType }, makePlayer } );
        netSystem.registerType(
                { 2, { &physicsNetStateType }, makeProjectile });

        playerInputSource.onPlayerInput.connect([&]
        (PlayerInput const& input) {
            playerInput = input;
        });
    }

    ~Client() {
        if (peer) {
            sendEvent<DisconnectWish>(peer);
            enet_host_flush(host);

            enet_host_destroy(host);
        }
    }

    void onKeyPressed(KeyInput const& input) {
        if (input.code == Key::P)
            ProfilingData::dump(); 
    }

    void onCreateEntityOrder(NetEntityTypeId type,
                             NetEntityId id,
                             ClientId owner) {
        auto entity = netSystem.createEntity(type, id, owner);

        ASSERT_MSG(myId > 0, "Should have received my id "
                             << "before CreateEntityOrder.");
        if (owner == myId && type == 1) {
            playerEntity = entity; 

            INFO(client) << "Created player entity: " << playerEntity;
        }
    }

    void onRemoveEntityOrder(NetEntityId id) {
        if (netSystem.exists(id)) {
            entities.remove(netSystem.get(id)->getEntity());
        }
        else
            WARN(client) << "Cannot delete net obj #" << id;
    }

    void onLoggedInOrder(ClientId id) {
        INFO(client) << "Logged into server with id " << (int)id;

        ASSERT(id > 0);
        myId = id;
    }

    void onPlayerPositionOrder(vec3 const& position) {
        // Prediction correction
#ifdef USE_PREDICTION
        if (playerEntity) {
            auto input =
                playerEntity->component<PlayerInputComponent>();
            if (input) {
                input->onCorrection(position);
            }
        }
#endif
    }

    void onPong() {
        if (!haveUnrepliedPing) {
            WARN(client) << "Got unwanted Pong";
            return;
        }

        INFO(client) << "Ping: " 
             << (clock.getElapsedTime() - timeSentLastPing).asMilliseconds();
        haveUnrepliedPing = false;
    }

    void connect(std::string const& hostName, enet_uint16 port) {
        INFO(client) << "Connecting to " << hostName << ":" << port;

        ENetAddress address;
        enet_address_set_host(&address, hostName.c_str());
        address.port = port;

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

        // Now we have new curState and nextStates

        //TRACE(client) << "@" << clock.getElapsedTime().asMilliseconds()
                      //<< ": Starting tick #" << curState.tick();

        tick = curState.tick();
        
        // Run events of this tick
        while (!eventQueue.empty() && eventQueue.frontTick() <= tick)
            eventQueue.popAndEmit(eventHub);

        netSystem.applyStates(curState);

        tickInterpolation = 0;
    }

    void interpolate() {
        PROFILE(interpolate);

        if (tick == 0)
            return;

        if (tickInterpolation >= 1) {
            //tickInterpolation = 1;
            startNextTick = true;
            /*std::cout << "@" << clock.getElapsedTime().asMilliseconds()
                      << ": end of tick" << std::endl;*/
            /*TRACE(client) << "@" << clock.getElapsedTime().asMilliseconds()
                          << ": End of tick";*/
            startTick();
            //TRACE(client) << "ohhh, ok?";
            //std::cout << "end interpolation start" << std::endl;
        }

        if (tickInterpolation >= 1.5)
            tickInterpolation = 1.5;
        netSystem.interpolateStates(curState, nextState, tickInterpolation);

        tickInterpolation += (float)TICK_FREQUENCY / INTERPOLATION_FREQUENCY;
    }

    void update(Time delta) {
        PROFILE(update);

        {
            PROFILE(receive);
            receive();
        }

        if (playerEntity) {
            auto physics = playerEntity->component<PhysicsComponent>();
            //if (physics)
                //SoundPlayer::setListenerPosition(physics->getPosition());
        }

        tasks.run(delta);

        if (!haveUnrepliedPing) {
            if (clock.getElapsedTime() - timeSentLastPing > seconds(1)) {
                timeSentLastPing = clock.getElapsedTime();
                sendEvent<Ping>(peer);
                haveUnrepliedPing = true;
            } 
        }
    }

    void render() {
        PROFILE(render);

        if (playerEntity) {
            auto playerPhys = playerEntity->component<PhysicsComponent>();

            vec3 cameraPosition = playerPhys->getPosition() +
                                  vec3(0, 15, -0.001);
            vec3 cameraTarget = playerPhys->getPosition();

            renderSystem.setCamera(cameraPosition, cameraTarget);
        }
        else
            renderSystem.setCamera(vec3(0, 8, -0.001), vec3(0, 0, 0));

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(renderSystem.getView()));
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(renderSystem.getProjection()));

        /*glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(2.0, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 2.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 2.0);
        glEnd();*/

        visionSystem.renderVision(renderSystem.getProjection(),
                                  renderSystem.getView());
        //visionSystem.getTexture().saveToFile("vision.png");

        groundRenderer.render(renderSystem.getProjection(),
                              renderSystem.getView());
        renderSystem.render();

        window.pushGLStates();
        sf::Text text("hello world", font);
        text.setPosition(100, 100);
        text.setColor({ 255, 0, 0 });

        window.draw(text);
        window.popGLStates();

        window.display();

        checkGLError();
    }

protected:
    // Implement ENetReceiver
    void onPacket(int channelId, ENetPeer*, ENetPacket* packet) {
        if (channelId == CHANNEL_MESSAGES || channelId == CHANNEL_TIME)
            emitEventFromPacket(eventHub, packet);
        else if (channelId == CHANNEL_TICKS) {
            BitStreamReader stream(packet->data, packet->dataLength);
            Tick tick;
            read(stream, tick);
            ASSERT(tick > 0);

            //INFO(net) << "Received tick " << tick;

            while (eventQueue.addFromStream(tick, stream));

            tickStateQueue.emplace(tick);
            netSystem.readStates(stream, tickStateQueue.back());

            if (startNextTick)
                startTick();
        }
    }

    void onConnect(ENetPeer*) {
        INFO(client) << "Connected";
    }

    void onDisconnect(ENetPeer*) {
        INFO(client) << "Lost connection to server";
    }
};

ComponentList makeTeapot(NetEntityId id, ClientId owner) {
    auto physics = new PhysicsComponent();
    return {
        physics,
        new RenderCube(physics, vec3(1, 0, 0)),
        new NetComponent(0, id, owner, { new PhysicsNetState(physics) }),
        new PlayerComponent(nullptr)
    };
}

ComponentList makePlayer(NetEntityId id, ClientId owner) {
    static vec3 colors[] = {
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 0.3, 0.8),
        vec3(1, 0, 1),
        vec3(0, 1, 1),
        vec3(1, 1, 0),
        vec3(0, 0, 0)
    };

    auto physics = new PhysicsComponent();

    auto components = ComponentList {
        physics,
        new RenderPlayerComponent(physics, colors[owner % 7],
                                  client->textures),
        new NetComponent(1, id, owner, { new PhysicsNetState(physics) }),
        new PlayerComponent(nullptr)
    };

    if (client && owner == client->myId) {
        components.push_back(new VisionComponent(physics));
#ifdef USE_PREDICTION
        components.push_back(new PlayerInputComponent(physics));
#endif
    }

    return components;
}

ComponentList makeProjectile(NetEntityId id, ClientId owner) {
    auto physics = new PhysicsComponent();

    return {
        physics,
        new RenderCube(physics, vec3(1, 0, 1), 0.3),
        new NetComponent(2, id, owner, { new PhysicsNetState(physics) }),
        new ProjectileComponent(physics, ProjectileComponent::GLOBAL)
    };
}

ComponentList makeMapObject(MapObjectComponent const* object) {
    return {
        new RenderMapObjectComponent(object, client->textures)
    };
}

int main()
{
    Log::addSink(new ConsoleLogSink());
    Log::addSink(new FileLogSink("client.log"));
    //Log::setSeverityFilter("net", SEVERITY_INFO);

    initializeEventTypes();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Client",
            sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(false);
    //window.setMouseCursorVisible(false);

    // Now that we have an OpenGL context, we can initialize GLEW
    {
        auto err = glewInit();
        if (err != GLEW_OK) {
            WARN(opengl) << "Failed to initialize GLEW: "
                         << glewGetErrorString(err) << ".";
 
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
        //std::cout << "\nPlease enter IP of server to connect to: ";
        std::string host = "localhost";
        //std::getline(std::cin, host);

        client.connect(host, 40003);
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
