#ifndef ACTION_H
#define ACTION_H

#include<string>
#include<vector>

#include "ai.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "player.hpp"
#include "port.hpp"
#include "tile.hpp"
#include "unit.hpp"

#include "../../joueur/src/base_ai.hpp"
#include "../../joueur/src/attr_wrapper.hpp"

using namespace cpp_client::pirates;

class action
{
public:
    action(AI* p, Unit u)
    {
        _ai = p;
        _un = u;
    }

    virtual bool can_execute();

    virtual bool execute();

    virtual void move_towards();

    std::string name;
    std::string job;
protected:
    AI* _ai;
    Unit _un;
};

typedef action* Action;

void action_init(Action act);

#endif