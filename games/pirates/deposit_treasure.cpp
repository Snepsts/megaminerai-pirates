#include "deposit_treasure.hpp"

deposit_treasure::deposit_treasure(AI* aip, Unit un, std::string name) : action(aip, un)
{
    this->name = name;
}

bool deposit_treasure::can_execute()
{
    path = _ai->find_path(_un->tile, _ai->player->port->tile, _un);
    return path.size() == 0;
}

bool deposit_treasure::execute()
{
    if(can_execute())
    {
        _un->deposit();
    }
    else
    {
        move_towards();
    }
}

void deposit_treasure::move_towards()
{
    if (path.size() > 0)
    {
        // Move along the path if there is one
        _un->move(path[0]);
    }
}