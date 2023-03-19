#include <iostream>

// template with static member which is the same for all objects of this class

class Monostate
{
    static int id;
public:
    int getId() { return id; }
    void setId(int value) { id = value; }
};

// weird syntax of static member initialization
int Monostate::id = 0;

int main()
{
    Monostate one;
    std::cout << one.getId() << std::endl;
    one.setId(5);
    std::cout << one.getId() << std::endl;

    //two.id == one.id
    Monostate two;
    std::cout << two.getId() << std::endl;

}