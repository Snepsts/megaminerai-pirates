<<<<<<< HEAD
#include "action.h"
=======
#include "action.hpp"
#include <vector>
>>>>>>> 4cefc5fa2cbaf49b8f44eb4861d0d2626ceaa04e

// Global definition of a vector of possible actions
std::vector<Action> ACTION_VECTOR;

void action_init(Action act)
{
    ACTION_VECTOR.push_back(act);
}