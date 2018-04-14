#include "action.h"

// Global definition of a vector of possible actions
std::vector<Action> ACTION_VECTOR;

void action_init(Action act)
{
    ACTION_VECTOR.push_back(act);
}