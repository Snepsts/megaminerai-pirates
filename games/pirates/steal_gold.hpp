#ifndef __STEAL_GOLD__
#define __STEAL_GOLD__

#include "ai.hpp"
#include "action.hpp"

using namespace cpp_client::pirates;

class steal_gold : public action
{
    public:
    steal_gold(AI* aip, Unit u);
    
    bool can_execute();

    bool execute();

    void move_towards();
};
#endif