#include <iostream>
#include <sstream>
#include <concepts>

// decorator with template inheritance
// every decoration class inherited of its template class 
// we can compare different decorators in order we need
// and also can modify members and use additional functionality

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


// for c++20
// template<typename T>
// std::concept IsHuman = std::is_base_of<Human, T>::value;


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

template<typename T>
struct SpanishHuman : T
{
    static_assert(std::is_base_of<Human, T>::value,
    "Template argument must be a Human");

    std::string word;

    template<typename... Args>
    SpanishHuman(const std::string& w, Args ...args) : 
        T(std::forward<Args>(args)...), word{w}
    {
    }

    std::string str()
    {
        std::ostringstream oss;
        oss << T::str() << " and says: " << word;
        return oss.str();
    }

};

template<typename T>
struct MusicalHuman : T
{
    static_assert(std::is_base_of<Human, T>::value,
    "Template argument must be a Human");

    int songs;

    template<typename... Args>
    MusicalHuman(int s, Args ...args) : 
        T(std::forward<Args>(args)...), songs(s)
    {
    }

    std::string str()
    {
        std::ostringstream oss;
        oss << T::str() << " and wrote " << songs << " songs";
        return oss.str();
    }
    void sing() {std::cout << "Ratatatatatata\n" ;}
};

int main()
{
    Human h{"Dilan"};
    Engineer e{h, "chemistry"};
    Worker w{h, 34};

    SpanishHuman<Engineer> s{"Pardon!", e};
    SpanishHuman<MusicalHuman<Worker>> m {"Mersi!", 10, w};

    std::cout << s.str() << std::endl;
    std::cout << m.str() << std::endl;

    // access to members
    m.songs = 30;
    // access to functions - in SpanishHuman to MusicalHuman
    m.sing();

    std::cout << m.str() << std::endl;


}