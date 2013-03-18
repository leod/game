#pragma once

#include "core/Component.hpp"

namespace game {

struct Player;

struct PlayerComponent : public ComponentBase<PlayerComponent> {
    PlayerComponent(Player* player)
       : player(player) {
    }

    Player* getPlayer() const {
        return player;
    }

private:
    Player* player;
};

} // namespace game
