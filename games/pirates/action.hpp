#ifndef ACTION_H
#define ACTION_H
#include<string>

class action
{
public:
    virtual bool can_execute();

    virtual bool execute();

    virtual void move_towards();

    std::string name;
    std::string job;
private:
};

typedef action* Action;

void action_init(Action act);

#endif