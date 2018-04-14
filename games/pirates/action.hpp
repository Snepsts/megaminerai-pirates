#ifndef ACTION_H
#define ACTION_H
#include<string>
#include<vector>
#include "player.hpp"

using namespace cpp_client::pirates;

class action
{
public:
    action(Player p, Unit u)
    {
        pl = p;
        un = u;
    }

    virtual bool can_execute();

    virtual bool execute();

    virtual void move_towards();

    std::string name;
    std::string job;
protected:
    Player pl;
    Unit un;
};

typedef action* Action;

void action_init(Action act);

#endif