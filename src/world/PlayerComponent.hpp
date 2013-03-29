#pragma once

#include "core/Component.hpp"

namespace game {

struct Player;

struct PlayerComponent : public Component<PlayerComponent> {
    PlayerComponent(Player* player)
       : player(player), life(100) {
    }

    Player* getPlayer() const {
        return player;
    }

    bool hurt(int amount) {
        life -= amount;
        if (life < 0) life = 0;

        return life == 0;
    }

private:
    Player* player;
    int life;
};

} // namespace game
