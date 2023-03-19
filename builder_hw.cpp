#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include <utility>

using namespace std;

struct Code 
{
  string name;
  vector<pair<string, string>> fields;

  Code() {}
  Code(string& name) : name(name) {}
};

class CodeBuilder
{
  Code code;
public:
  CodeBuilder(const string& class_name)
  {
      code.name = class_name;
  }

  CodeBuilder& add_field(const string& name, const string& type)
  {
    code.fields.emplace_back(make_pair(name, type));
    return *this;
  }

  friend ostream& operator<<(ostream& os, const CodeBuilder& obj)
  {
    os << "class " << obj.code.name << endl;
    os << "{" <<  endl;
    if(obj.code.fields.size()) 
    {
      for(auto& f : obj.code.fields)
      {
        os << "  " << f.second << " " << f.first << ";" <<  endl;
      }
    }
    os << "};" <<  endl;
    return os;
  }
};

int main() {

    auto cb = CodeBuilder{"Person"}.add_field("name", "string").add_field("age", "int");
    cout << cb;
    
}

/*
class Person
    {
      string name;
      int age;
    };
*/