#include <iostream>
#include <sstream>

// decorator with inheritance
// every decoration class has a reference member of the base class type 
// and additional functionality
// we can compare different decorators in order we need

struct Entity
{
    virtual std::string str() = 0;
};

struct Human : Entity
{
    std::string name;

    Human(std::string n = "Default") : name(n)
    {}

    std::string str()
    {
        std::ostringstream oss;
        oss << "This human has a name " << name;
        return oss.str();
    }
};

struct Engineer : Human
{
    Human& human;
    std::string field;

    Engineer(Human& h, std::string f = "physics") : human{h}, field(f)
    {
    }

    std::string str()
    {
        std::ostringstream oss;
        oss << human.str() << " and works as an engineer in " << field;
        return oss.str();
    }

};

struct Worker : Human
{
    Human& human;
    int time;

    Worker(Human& h, int t = 40) : human{h}, time(t)
    {
    }

    std::string str()
    {
        std::ostringstream oss;
        oss << human.str() << " and works " << time << " hours per week";
        return oss.str();
    }

    void work() {std::cout << "WOOOOOOOOOOORK!!!\n";}

};

struct SpanishHuman : Human
{
    Human& human;
    std::string word;

    SpanishHuman(Human& h, std::string w = "Ola!") : human{h}, word(w)
    {
    }

    std::string str()
    {
        std::ostringstream oss;
        oss << human.str() << " and says: " << word;
        return oss.str();
    }

};

int main()
{
    Human h{"Dilan"};
    Engineer e{h, "chemistry"};
    Worker w{h, 34};

    std::cout << h.str() << std::endl;
    std::cout << e.str() << std::endl;
    std::cout << w.str() << std::endl;

    Human a{"Tony"};
    Engineer b{a, "chemistry"};
    Worker c{a, 34};
    SpanishHuman d{b, "Mersi!"};
    SpanishHuman f{c, "Gata!"};
    SpanishHuman g{f, "Gato?"};
    Worker i{b, 43};

    std::cout << a.str() << std::endl;
    std::cout << b.str() << std::endl;
    std::cout << c.str() << std::endl;
    std::cout << d.str() << std::endl;
    std::cout << f.str() << std::endl;
    std::cout << g.str() << std::endl;
    std::cout << i.str() << std::endl;

// the main issue: we can't use additional functions of derived classes after decoration 
// f.work() is impossible to call;

}