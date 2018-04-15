#include "steal_gold.hpp"

    steal_gold::steal_gold(AI* aip, Unit u) : action(aip, u)
    {
        name = "steal_gold";
    }
    
    bool steal_gold::can_execute()
    {
        if(_un->tile->gold == 0)
        {
            std::vector<Tile> temp =_ai->build_list_of_enemy_treasure();
            if(temp.size() == 0)
                return false;
        }
        return true;
    }

    bool steal_gold::execute()
    {
        if (can_execute() && _un->tile->gold > 0) {
            _un->dig(-1);
        }
        else {
            move_towards();
        }
    }

    void steal_gold::move_towards()
    {
        _ai->move_towards_enemy_treasure(_un);
    }