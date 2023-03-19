#include <iostream>
#include <functional>
#include <map>
#include <memory>


struct HotDrink 
{
    virtual ~HotDrink() = default;
    virtual void prepare(int16_t) = 0;
};

struct Coffee : HotDrink
{
    void prepare(int16_t volume) 
    {
        std::cout << "Preparing " << volume << " ml coffee\n";
    }
};

struct Tea : HotDrink
{
    void prepare(int16_t volume) 
    {
        std::cout << "Preparing " << volume << " ml tea\n";
    }
};

struct HotDrinkFactory 
{
    virtual ~HotDrinkFactory() = default;
    virtual std::unique_ptr<HotDrink> make() = 0;
};

struct CoffeeFactory : HotDrinkFactory
{
    std::unique_ptr<HotDrink> make() 
    {
        return std::make_unique<Coffee>();
    }
};

struct TeaFactory : HotDrinkFactory
{
    std::unique_ptr<HotDrink> make() 
    {
        return std::make_unique<Tea>();
    }
};

class DrinksFactory
{
    std::map<std::string, std::unique_ptr<HotDrinkFactory>> factory;
public:
    DrinksFactory()
    {
        factory["tea"] = std::make_unique<TeaFactory>();
        factory["coffee"] = std::make_unique<CoffeeFactory>();
    }

    std::unique_ptr<HotDrink> make(std::string choice) 
    {
        return factory[choice]->make();
    }
};

class LambdaDrinkFactory
{
    std::map<std::string, std::function<std::unique_ptr<HotDrink>()>> factory;
public:
    LambdaDrinkFactory()
    {
        factory["tea"] = [] {
            auto t = std::make_unique<Tea>();
            t->prepare(100);
            return t;
        };

        factory["coffee"] = [] {
            auto t = std::make_unique<Coffee>();
            t->prepare(200);
            return t;
        };
    }

    std::unique_ptr<HotDrink> make(std::string choice) 
    {
        return factory[choice]();
    }
};

int main()
{
    DrinksFactory df;
    df.make("tea")->prepare(100);
    df.make("coffee")->prepare(200);

    LambdaDrinkFactory ldf;
    ldf.make("tea");
    ldf.make("coffee");
}