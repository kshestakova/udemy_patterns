#include <iostream>
#include <string>
#include <vector>

// interface that we have
// it can work only with vectors of int values
class Adaptee
{
public:
    std::string AdapteeRequest(const std::vector<int>& a) const 
    { 
        std::cout << " AdapteeRequest interface usage\n";
        std::string s = "";
        for(auto i : a)
        {
            s += std::to_string(i) + "\n";
        }
        return  s; 
    }
};

// third-party interface which we should use
// it can only work with int values
class Target
{
public:
    virtual ~Target() = default;
    virtual std::string TargetRequest(int a, int b, int c) const 
    {
        std::cout << " TargetRequest interface usage\n";
        return std::to_string(a) + "\n" + std::to_string(b) + "\n" + std::to_string(c) + "\n"; 
    }
};

// class which adaptates Target interface to Adaptee interface
class Adapter : public Target
{
    Adaptee *adaptee;
public:
    Adapter(Adaptee* a) : adaptee(a) {}
    std::string TargetRequest(int a, int b, int c) const 
    { 
        std::cout << "TargetRequest interface usage in Adapter class\n";
        std::vector<int> v;
        v.emplace_back(a);
        v.emplace_back(b);
        v.emplace_back(c);
        return this->adaptee->AdapteeRequest(v);
    }
};

int main()
{
    // Target class works with (int, int, int)
    Target *target = new Target;
    std::cout << target->TargetRequest(1, 2, 3) << "\n";

    // Adaptee class works with std::vector<int>
    Adaptee *adaptee = new Adaptee;
    std::vector<int> v {4, 5, 6};
    std::cout << adaptee->AdapteeRequest(v) << "\n";
    
    // Adapter makes possible working with (int, int, int) for object of Adaptee class
    // which was cheates for std::vector<int>

    // In fact here we create Adapter object based on Adaptee
    // and use  Target class method for it
    Adapter *adapter = new Adapter(adaptee);
    std::cout << adapter->TargetRequest(1, 2, 3) << "\n";

    delete target;
    delete adaptee;
    delete adapter;

}