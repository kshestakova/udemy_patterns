#include <iostream>
#include <vector>
#include <algorithm> // for std::remove
#include <mutex>

template<typename T>
class Observer;

template<typename T>
class ThreadSafeObservable
{
    std::vector<Observer<T>*> observers;
    std::mutex mtx; // alternative - recursive_mutex
public:
    void notify(T& object, const std::string& fieldName)
    {
        std::scoped_lock{mtx}; // releases automatically after the scope
        for(auto it : observers)
        {
            if(it) // to avoid calling notification for nullptr
                it->fieldChanged(object, fieldName);
        }
    }

    void subscribe(Observer<T>& observer)
    {
        std::scoped_lock{mtx};
        observers.emplace_back(&observer);
    }

    void unsubscribe(Observer<T>& observer)
    {
        std::scoped_lock{mtx};
        auto it = std::find(observers.begin(), observers.end(), &observer);
        *it = nullptr; // instead of erasing - to avoid deadlock 
    }
};

class Person : public ThreadSafeObservable<Person>
{
    std::string name;
    int age;
public:
    Person(const std::string& name, const int age) : name(name), age(age) {}
    void setAge (const int a) 
    {
        if(age != a)
        {
            auto oldVotingStatus = getVotingStatus();
            age = a; 
            notify(*this, "age");

            if(oldVotingStatus != getVotingStatus())
            {
                notify(*this, "votingStatus");
            }
        } 
        
    }
    int getAge() const { return age; }

    void setName (const std::string& n) 
    { 
        if(name != n)
        {
            name = n;
            notify(*this, "name");
        } 
    }
    std::string getName() const { return name; }
    
    bool getVotingStatus() const { return age >= 18; }
};

template<typename T>
class Observer
{
public:
    virtual void fieldChanged(T& object, const std::string& fieldName) = 0;
    virtual ~Observer() = default;
};

class TrafficAdministration : public Observer<Person>
{
public:
    void fieldChanged(Person& object, const std::string& fieldName) override
    {
        if(fieldName == "age") 
        {
            if(object.getAge() <= 16) std::cout << "TrafficAdministration notifies: you are too young to drive\n";
            else
            {
                std::cout << "TrafficAdministration notifies: you can pass a driving test\n";
                object.unsubscribe(*this);
            }
        }
    }
};

int main()
{
    Person p("Ted", 14); // creaing observable
    TrafficAdministration ta;   // and observer

    p.subscribe(ta);     // now we'll see changes

    p.setAge(15);
    p.setAge(16);
    p.setAge(18);
    p.setAge(20); // here we have no observers and no messages

}