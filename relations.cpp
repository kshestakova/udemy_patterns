#include <iostream>
#include <string>
#include <tuple>
#include <vector>

enum class Relations {
    parent,
    child,
    sibling
};

struct Person {
    std::string name;
};

struct RelationshipBrowser {
    virtual std::vector<Person> getAllChildren(std::string& parentName) = 0;
};

struct Relationship : RelationshipBrowser {
    std::vector<std::tuple<Person, Relations, Person>> relationships;

    void addRelationship(Person& parent, Person& child) {
        relationships.push_back({parent, Relations::parent, child});
        relationships.push_back({child, Relations::child, parent});
    }

    std::vector<Person> getAllChildren(std::string& parentName) {
        std::vector<Person> result;
        for(auto& [parent, relation, child] : relationships) {
            if(parent.name == parentName && relation == Relations::parent) {
                result.push_back({child});
            }
        }
        return result;
    }
};

struct Research {
    Research(RelationshipBrowser& browser, std::string name)
    {
        for (auto& child : browser.getAllChildren(name))
        {
            std::cout << name << " has a child called " << child.name << std::endl;
        }
    }
};

int main() {
    Person Tom{"Tom"};
    Person Tim{"Tim"};
    Person Michael{"Michael"};
    Person Anna{"Anna"};

    Relationship family;
    family.addRelationship(Tom, Tim);
    family.addRelationship(Tom, Michael);
    family.addRelationship(Michael, Anna);

    Research forTom(family, "Tom");
    Research forMichael(family, "Michael");
};