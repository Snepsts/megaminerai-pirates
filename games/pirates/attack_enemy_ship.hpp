#ifndef __DEFEND_TREASURE__
#define __DEFEND_TREASURE__

#include "ai.hpp"
#include "action.hpp"

using namespace cpp_client::pirates;

class attack_enemy_ship : public action
{
    bool can_execute();

    bool execute();

};

#endif