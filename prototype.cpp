#include <iostream>
#include <sstream>
#include <memory>

struct Address
{
    std::string street;
    int16_t building;

    Address(std::string street, int16_t building) : street(street), building(building) {}
    Address(const Address &other) : street(other.street), building(other.building) {}

    friend std::ostream& operator<<(std::ostream& os, const Address& address)
    {
        os << "street: " << address.street << ", building: " << address.building << std::endl;
        return os;
    }
};

struct Contact
{
    std::string name;
    Address* address;

    Contact(std::string name, Address* other) : name(name), address(new Address{other->street, other->building}) {}

    friend std::ostream& operator<<(std::ostream& os, const Contact& contact)
    {
        os << "Name: " << contact.name << ", address: " << *contact.address << std::endl;
        return os;
    }
};

struct EmployeeFactory
{
    static std::unique_ptr<Contact> newMainOfficeEmployee(const std::string& name, const int16_t building)
    {
        static Contact mainOfficePrototype {"", new Address("Glushko", 0)};
        return newEmployee(name, building, mainOfficePrototype);
    }
    static std::unique_ptr<Contact> newChildOfficeEmployee(const std::string& name, const int16_t building)
    {
        static Contact childOfficePrototype {"", new Address("Koroleva", 0)};
        return newEmployee(name, building, childOfficePrototype);
    }
private:
    static std::unique_ptr<Contact> newEmployee(const std::string& name, const int16_t building, const Contact& prototype)
    {
        auto result = std::make_unique<Contact>(prototype);
        result->name = name;
        result->address->building = building;
        return result;
    }
};

int main()
{
    Contact michael{"Michael", new Address{"Ilfa i Petrova", 105}};
    Contact john{michael};

    std::cout << michael;
    std::cout << john;

    john.address->building = 1001;
    john.name = "John";
    std::cout << john;

    auto bill = EmployeeFactory::newMainOfficeEmployee("Bill from the main office", 15);
    std::cout << *bill;

    auto buckster = EmployeeFactory::newChildOfficeEmployee("Buckster from the child office", 19);
    std::cout << *buckster;
}