#include <iostream>
#include <string>
#include <vector>

enum class Color {red, yellow, green, blue};
enum class Size {small, medium, large};

struct Product
{
    std::string name;
    Color color;
    Size size;

    Product(const std::string& name, const Color& color, const Size& size) : name(name), color(color), size(size)
    {}
};

struct SimpleFilter
{
    std::vector<Product*> byColor(std::vector<Product*> products, Color color)
    {
        std::vector<Product*> result;
        for(auto& p : products)
        {
            if(p->color == color)
            {
                result.push_back(p);
            }
        }
        return result;
    }

    // not good - copypaste + changes in the interface
    std::vector<Product*> bySize(std::vector<Product*> products, Size size)
    {
        std::vector<Product*> result;
        for(auto& p : products)
        {
            if(p->size == size)
            {
                result.push_back(p);
            }
        }
        return result;
    }

    // not good - changes in the interface + too much copypaste if number of criteria increases
    // also this way breaks Open Close Principle form SOLID
    std::vector<Product*> bySizeAndColor(std::vector<Product*> products, Size size, Color color)
    {
        std::vector<Product*> result;
        for(auto& p : products)
        {
            if(p->size == size && p->color == color)
            {
                result.push_back(p);
            }
        }
        return result;
    }
};

template<typename T>
struct AndSpecification;

template<typename T>
struct Specification
{
    virtual bool isSatisfied(T* item) = 0;

    // to combine two differrent specifications
    AndSpecification<T> operator&&(Specification<T>&& otherSpecification)
    {
        return AndSpecification<T>(*this, otherSpecification);
    }
};

template<typename T>
struct Filter
{
    virtual std::vector<T*> filter(std::vector<T*> items, Specification<T>& specification) = 0;
};

struct ColorSpecification : Specification<Product>
{
    Color color;

    ColorSpecification(Color color) : color(color)
    {}

    bool isSatisfied(Product* product) override
    {
        return product->color == color;
    }
};

// new filter specifications without interface changes
struct SizeSpecification : Specification<Product>
{
    Size size;

    SizeSpecification(Size size) : size(size)
    {}

    bool isSatisfied(Product* product) override
    {
        return product->size == size;
    }
};

// pretty simple creation if filter combinations
template<typename T>
struct AndSpecification : Specification<T>
{
    Specification<T>& first;
    Specification<T>& second;

    AndSpecification(Specification<T>& first, Specification<T>& second) : first(first), second(second)
    {}

    bool isSatisfied(T* item) {
        return first.isSatisfied(item) && second.isSatisfied(item);
    }
};

struct SmartFilter : Filter<Product>
{
    std::vector<Product*> filter(std::vector<Product*> items, Specification<Product>& specification) override
    {
        std::vector<Product*> result;
        for(auto i : items)
        {
            if(specification.isSatisfied(i))
                result.push_back(i);
        }
        return result;
    }
};

int main() {

    Product one{"lime", Color::green, Size::small};
    Product two{"banana", Color::yellow, Size::small};
    Product three{"watermelon", Color::green, Size::large};
    Product four{"apple", Color::red, Size::medium};

    std::vector<Product*> products {&one, &two, &three, &four};

    SimpleFilter sf;

    auto greenProducts = sf.byColor(products, Color::green);

    for(auto& gp : greenProducts)
    {
        std::cout << gp->name << " is green.\n";
    }

    auto smallProducts = sf.bySize(products, Size::small);

    for(auto& sp : smallProducts)
    {
        std::cout << sp->name << " is small.\n";
    }

    auto smallYellowProducts = sf.bySizeAndColor(products, Size::small, Color::yellow);

    for(auto& sp : smallYellowProducts)
    {
        std::cout << sp->name << " is small and yellow.\n";
    }

    SmartFilter smf;
    ColorSpecification cs(Color::green);
    SizeSpecification ss(Size::small);

    auto smartFilteredGreenProducts = smf.filter(products, cs);
    for(auto& sp : smartFilteredGreenProducts)
    {
        std::cout << sp->name << " is green (by smart filter).\n";
    }

    auto smartFilteredSmallProducts = smf.filter(products, ss);
    for(auto& sp : smartFilteredSmallProducts)
    {
        std::cout << sp->name << " is small (by smart filter).\n";
    }

    AndSpecification<Product> as(cs, ss);

    auto smallAndGreenProducts = smf.filter(products, as);
    for(auto& sp : smallAndGreenProducts)
    {
        std::cout << sp->name << " is small and green (by smart filter).\n";
    }

    //the most compact way to use it
    for(auto& sp : smf.filter(products, ColorSpecification(Color::yellow) && SizeSpecification(Size::small)))
    {
        std::cout << sp->name << " is small and yellow (by smart filter).\n";
    }

    return 0;
}
