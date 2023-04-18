#include <iostream>
#include <vector>
#include <algorithm> // for std::remove
#include <mutex>

template<typename T>
class Observer;

template<typename T>
class Observable
{
    std::vector<Observer<T>*> observers;
public:
    void notify(const T& object, const std::string& fieldName)
    {
        for(auto it : observers)
        {
            it->fieldChanged(object, fieldName);
        }
    }

    void subscribe(Observer<T>& observer)
    {
        observers.emplace_back(&observer);
    }

    void unsubscribe(Observer<T>& observer)
    {
        observers.erase(
            std::remove(observers.begin(), observers.end(), &observer), 
            observers.end()); 
    }
};

class Person : public Observable<Person>
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
    virtual void fieldChanged(const T& object, const std::string& fieldName) = 0;
    virtual ~Observer() = default;
};

class PersonObserver : public Observer<Person>
{
public:
    void fieldChanged(const Person& object, const std::string& fieldName) override
    {
        std::cout << "Field changed: " << fieldName << std::endl;
        if(fieldName == "name") std::cout << object.getName() << std::endl;
        else if(fieldName == "age") std::cout << object.getAge() << std::endl;
        else if(fieldName == "votingStatus") std::cout << std::boolalpha << object.getVotingStatus() << std::endl; // not actual field but notification
        else std::cout << "But this fiels doesn't exsist in the Person class" << std::endl; 
    }
};

int main()
{
    Person p("Ted", 35); // creaing observable
    PersonObserver po;   // and observer

    p.subscribe(po);     // now we'll see changes

    p.setName("Pablo");
    p.setName("Pablo");  // name doesn't change - no std::cout

    p.setAge(13);
    p.setAge(56);

    p.unsubscribe(po);
    p.setAge(39);       // no observers - no std::cout
}