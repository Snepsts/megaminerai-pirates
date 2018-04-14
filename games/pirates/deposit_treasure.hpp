#ifndef __DEPOSIT_TREASURE__
#define __DEPOSIT_TREASURE__

#include "ai.hpp"
#include "action.hpp"

using namespace cpp_client::pirates;

class deposit_treaser : public action
{
public:
    bool can_execute()
    {
        
    }

    bool execute()
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

    void move_towards()
    {
        _ai->find_path(_un->tile, _ai->player->port->tile, _un);
        //this->find_path(unit->tile, this->player->port->tile, unit);
    }
private:

};

#endif