#include <iostream>
#include <string>
#include <vector>

using namespace std;

class PersonBuilder;

class Person 
{
    string homeAddress;
    string postCode;
    string phoneNumber;
    string company;
    string officeAddress;
    string annualIncome;

public:
    static PersonBuilder create() 
    {
        return PersonBuilder();
    }

    string str() 
    {
        return  "Person lives at " + homeAddress + " with postCode " + postCode + " and phoneNumber " + phoneNumber 
        + ".\n Works for company " + company + "in the office " + officeAddress + " and earns $" + annualIncome + "a year.\n";
    }

    friend class PersonBuilder;
    friend class PersonHomeBuilder;
    friend class PersonWorkBuilder;
};

class PersonWorkBuilder;
class PersonHomeBuilder;

class PersonBuilderBase
{
protected:
    Person& person;

    explicit PersonBuilderBase(Person& person) : person(person) 
    {}

public:
    operator Person() const
    {
        return move(person);
    }

    PersonWorkBuilder works() const {return PersonWorkBuilder{person};}
    PersonHomeBuilder lives() const {return PersonHomeBuilder{person};}
};

class PersonBuilder : public PersonBuilderBase
{
    Person person;

public:
    PersonBuilder() : PersonBuilderBase(person)
    {}

};

class PersonHomeBuilder : public PersonBuilderBase
{
public:
    explicit PersonHomeBuilder(Person& person) : PersonBuilderBase(person) 
    {}

    PersonHomeBuilder& at(string homeAddress) 
    {
        person.homeAddress = homeAddress;
        return *this;
    }

    PersonHomeBuilder& withCode(string postCode) 
    {
        person.postCode = postCode;
        return *this;
    }

    PersonHomeBuilder& withPhone(string phoneNumber) 
    {
        person.phoneNumber = phoneNumber;
        return *this;
    }
};

class PersonWorkBuilder : public PersonBuilderBase
{
public:
    explicit PersonWorkBuilder(Person& person) : PersonBuilderBase(person) 
    {}
public:
    PersonWorkBuilder& in(string company) 
    {
        person.company = company;
        return *this;
    }

    PersonWorkBuilder& at(string officeAddress) 
    {
        person.officeAddress = officeAddress;
        return *this;
    }

    PersonWorkBuilder& with(string annualIncome) 
    {
        person.annualIncome = annualIncome;
        return *this;
    }
};

int main() 
{
    Person p = Person::create()
        .lives().at("15th street").withCode("42352").withPhone("243243143543")
        .works().in("Microsoft").at("Victory ave., 13").with("82000");

        cout << p.str();
}