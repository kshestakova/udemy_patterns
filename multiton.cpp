#include <iostream>
#include <map>
#include <memory>

enum class Priority
{
    first,
    second,
    third
};

template<typename T, typename K = std::string> 
class Multiton
{
    static std::map<K, std::shared_ptr<T>> instances;
protected:
    Multiton() = default;
    virtual ~Multiton() = default;
public:
    static std::shared_ptr<T> get(const K& key)
    {
        if( const auto it = instances.find(key); it != instances.end() )
        {
            return it->second;
        }

        auto i = std::make_shared<T>();
        instances[key] = i;
        return i;
    }
};

template<typename T, typename K> 
std::map<K, std::shared_ptr<T>> Multiton<T, K>::instances;

class Printer
{   
    static int count;
public:
    Printer() 
    {
        ++count;
        std::cout << "Now we have " << count << " printers\n";
    }

};

int Printer::count = 0;

int main()
{
    // we can create only one printer of each Priority
    // in map can be only one value with unique key
    // so we can have a class for multiple same-type objects, each for one enum or other unique key value

   auto one = Multiton<Printer, Priority>::get(Priority::first);
   auto two = Multiton<Printer, Priority>::get(Priority::first);

   auto three = Multiton<Printer, Priority>::get(Priority::second);
   auto four = Multiton<Printer, Priority>::get(Priority::second);

   auto five = Multiton<Printer, Priority>::get(Priority::third);
   auto six = Multiton<Printer, Priority>::get(Priority::third);
}
