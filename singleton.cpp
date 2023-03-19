#include <fstream>
#include <iostream>
#include <map>

class Singleton
{
    Singleton() 
    {
        std::ifstream is;
        is.open("prices.txt");
        std::string name, price;
        int16_t iPrice;
        while(getline(is, name))
        {
            getline(is, price);
            iPrice = std::stoi(price);
            pricebook[name] = iPrice;
        }
        is.close();
    }

    std::map<std::string, int16_t> pricebook;
public:
    Singleton(const Singleton& s) = delete;
    void operator=(const Singleton& s) = delete;
    static Singleton& get() 
    {
        static Singleton s;
        return s;
    }
    int16_t getPrice(const std::string& name) {return pricebook[name];}
};

int main()
{
    std::string s1 = "Avocado";
    std::cout << s1 << " " << Singleton::get().getPrice(s1) << std::endl;
    s1 = "Mango";
    std::cout << s1 << " " << Singleton::get().getPrice(s1) << std::endl;
}