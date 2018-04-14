#include "action.hpp"
#include <vector>

std::vector<Action> ACTION_VECTOR;

void action_init(Action act)
{
    ACTION_VECTOR.push_back(act);
}