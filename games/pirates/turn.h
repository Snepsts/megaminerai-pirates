#ifndef TURN_H
#define TURN_H
#include "action.h"
#include "unit.hpp"
#include <string>
#include <vector>

using namespace cpp_client::pirates;

Action get_action(Unit u)
{
    std::vector<Action> filteredActions = filter_action_by_job(unit);
}

#endif