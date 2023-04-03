#include <iostream>
#include <map>

typedef uint32_t key;

class User
{
    key firstName;
    key lastName;

    static std::map<key, std::string> names;

    key findByValue(std::string v);

public:
    User(std::string firstName, std::string lastName) : 
        firstName(findByValue(firstName)), 
        lastName(findByValue(lastName))
    {
    }

    key add(std::string value)
    {
        key k = names.size();
        names[k] = value;
        return k;
    }

    static void printMap() 
    {
        for(auto it : names)
        {
            std::cout << it.first << " " << it.second << std::endl;
        }
    }

    friend std::ostream& operator<<(std::ostream& oss, User& user);
};

std::ostream& operator<<(std::ostream& oss, User& user)
{
    oss << user.firstName << "\t" << user.names[user.firstName]  << "\t" 
        << user.lastName << "\t" << user.names[user.lastName] << std::endl;
    return oss;
}

std::map<key, std::string> User::names{};

key User::findByValue(std::string v)
{
    for(auto it : names)
    {
        if(it.second == v)
        {
            return it.first;
        }
    }
    return add(v);
}

int main() 
{
    User user1{"John", "Doe"};
    User user2{"Jane", "Doe"};

    User::printMap();
    std::cout << user1 << user2;

}