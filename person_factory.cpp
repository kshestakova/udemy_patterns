/*
You are given a class called Person . The person has two fields: id , and name .

Please implement a non-static PersonFactory that has a create_person()  method that takes a person's name.

The id  of the person should be set as a 0-based index of the object created. 
So, the first person the factory makes should have id=0, second id=1 and so on.
*/

#include <string>
using namespace std;

struct Person
{
  int id;
  string name;

  Person(int id, const string& name) : id(id), name(name) {}
};

class PersonFactory
{
  static int16_t counter;
public:
  Person create_person(const string& name)
  {
    return Person(counter++, name);
  }
};

int16_t PersonFactory::counter = 0;

int main() 
{
    PersonFactory pf;
    Person one = pf.create_person("one");
    Person two = pf.create_person("two");
}