#ifndef ACTION_H
#define ACTION_H

class action
{
public:
    virtual bool can_execute();

    virtual bool execute();

    virtual void move_towards();
private:
};

typedef action* Action;

void action_init(Action act);

#endif