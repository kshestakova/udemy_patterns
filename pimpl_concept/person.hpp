#pragma once
#include <iostream>
#include <string>

class Person
{
    std::string name;
public:
    Person(std::string n = "Default");
    ~Person();
    class PersonImpl;
    PersonImpl* impl;

    void greet();
    std::string getName();
    
    // incapsulation: user could use this functionality 
    // but doesn't get details of implementation
    std::string secret();
    
    // advantages:
    // separate interface and implementation
    // show interface, hide realization
    // possibility to change the realization and recompile it without changing interface file
};