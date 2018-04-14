#ifndef __DEPOSIT_TREASURE__
#define __DEPOSIT_TREASURE__

#include "ai.hpp"
#include "action.hpp"

using namespace cpp_client::pirates;

class deposit_treaser : action
{
public:
    bool can_execute()
    {
        
    }

    bool execute();

    void move_towards();
private:

}

#endif