#ifndef __DEPOSIT_TREASURE__
#define __DEPOSIT_TREASURE__

#include "ai.hpp"
#include "action.hpp"
#include <string>

using namespace cpp_client::pirates;

class deposit_treasure : public action
{
public:
    deposit_treasure(AI* aip, Unit un, std::string name);

    bool can_execute();

    bool execute();

    void move_towards();
    
private:
    std::vector<Tile> path;
};

#endif