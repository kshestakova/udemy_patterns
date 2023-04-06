#include <iostream>

template<typename T>
struct Proxy
{
    T value;

    // assignment operator override
    T& operator=(const T& v)
    {
        std::cout << "Assignment operator\n";
        return value = v;
    }

    operator T()
    {
        return value;
    }

    // constructor which uses overloaded assignment operator
    Proxy(const T& v)
    {
        std::cout << "Constructor\n";
        *this = v;
    }
};

struct Creature
{
    Proxy<int> strength{10};
    Proxy<int> health{15};

    std::string str()
    {
        return std::to_string(strength) + "\t" + std::to_string(health) + "\n";
    }

};

int main()
{
    Creature c;
    c.health = 8;
    c.strength = 10;
    std::cout << c.str(); 

    Creature b = c;
    std::cout << b.str(); 
}