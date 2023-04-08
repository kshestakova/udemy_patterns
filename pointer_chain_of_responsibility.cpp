#include <iostream>

struct Creature
{
    int health;
    int strength;
    Creature(int h, int s) : health(h), strength(s) {}
    friend std::ostream& operator<<(std::ostream& os, const Creature& c)
    {
        os << c.health << "\t" << c.strength << "\n";
        return os;
    }
};

class CreatureModifier
{
    CreatureModifier* next{nullptr};
protected:
    Creature& creature;
public:
    explicit CreatureModifier(Creature& c) : creature(c) {} 

    void add(CreatureModifier* cm)
    {
        if(next)
        {
            next->add(cm);
        }
        else
        {
            next = cm;
        }
    }

    virtual void handle()
    {
        if(next) next->handle();
    }

    virtual ~CreatureModifier() = default;
};

class ZeroModifier : public CreatureModifier
{
public:
    explicit ZeroModifier(Creature& c) : CreatureModifier(c) {} 
    void handle() {}
};

class HealthModifier : public CreatureModifier
{
public:
    explicit HealthModifier(Creature& c) : CreatureModifier(c) {} 
    void handle() 
    {
        creature.health *= 3;
        CreatureModifier::handle();
    }
};

class StrengthModifier : public CreatureModifier
{
public:
    explicit StrengthModifier(Creature& c) : CreatureModifier(c) {}
    void handle() 
    {
        if(creature.health > 5) creature.strength += 10;
        CreatureModifier::handle();
    }
};

int main()
{
    Creature creature{3, 5};
    CreatureModifier root{creature};

    StrengthModifier s1{creature}, s2{creature};
    HealthModifier h{creature};
    ZeroModifier z{creature};

    // making chain of responsibility
    root.add(&s1);
    root.add(&s2);
    root.add(&h);

    std::cout << creature;
    root.handle();
    std::cout << creature;
    root.handle();
    std::cout << creature;

    CreatureModifier root2{creature};
    root2.add(&z); // all other next modifiers won't work
    root2.add(&s2);
    root2.add(&h);
    std::cout << creature;


}