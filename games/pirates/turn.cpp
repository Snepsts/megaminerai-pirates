#include "turn.hpp"
extern std::vector<Action> ACTION_VECTOR;

using namespace cpp_client::pirates;
std::vector<Action> filter_action_by_job(Unit u)
{
    std::vector<Action> filtered;
    for(Action entry : ACTION_VECTOR)
    {
        if() //decide how to filter
        {
            filtered.push_back(entry);
        }
    }
}