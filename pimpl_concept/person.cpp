#include "person.hpp"

class Person::PersonImpl
{
public:
    void greet(Person* p);
};

void Person::PersonImpl::greet(Person* p)
{
    std::cout << "Hello, " << p->getName() << std::endl;
}

Person::Person(std::string n) : impl (new PersonImpl()), name(n)
{
}

Person::~Person()
{
    delete impl;
}

std::string Person::getName()
{
    return name;
}

void Person::greet()
{
    impl->greet(this);
}

std::string Person::secret()
{
    return "Secret string \n";
}