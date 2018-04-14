#ifndef ACTION_H
#define ACTION_H
#include<string>
#include<vector>
#include "unit.hpp"
#include "ai.hpp"

using namespace cpp_client::pirates;

class action
{
public:
    action(AI *p, Unit u)
    {
        _ai = p;
        _un = u;
    }

    virtual bool can_execute();

    virtual bool execute();

    virtual void move_towards();

    std::string name;
    std::string job;
protected:
    AI* _ai;
    Unit _un;
};

typedef action* Action;

void action_init(Action act);

#endif