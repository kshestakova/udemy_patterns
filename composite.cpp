#include <iostream>
#include <vector>

// the aim of using composite - to build constructions from their parts
// and to treat the whole construction and each its part in the same manner

struct Cat
{
    virtual void meow() = 0;
};

struct BlackCat : Cat
{
    std::string name;

    BlackCat(std::string name) : name{name} {}
    void meow() { std::cout << name << " said MEOW\n"; }
};

struct WhiteCat : Cat
{
    std::string name;

    WhiteCat(std::string name) : name{name} {}
    void meow() { std::cout << "MEOW said " << name << "\n"; }
};

struct GroupOfCats : Cat
{
    std::vector<Cat*> cats;
    std::string name;

    GroupOfCats(std::string name) : name{name} {}
    void meow() 
    { 
        std::cout << "GroupOfCats " << name << " say MEOW\n"; 
        for(auto&& c : cats)
        {
            c->meow();
        }
    }
};

int main()
{
    BlackCat b{"Bourbon"};
    WhiteCat w{"Whiskey"};

    GroupOfCats g("BigGroup");
    g.cats.emplace_back(&b);
    g.cats.emplace_back(&w);

    GroupOfCats sg("SubGroup");
    sg.cats.emplace_back(&b);
    sg.cats.emplace_back(&w);
    
    g.cats.emplace_back(&sg);

    g.meow();

}