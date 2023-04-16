#include <iostream>
#include <vector>
#include <algorithm>

class ChatRoom;

class Person
{
    std::string name;
    ChatRoom* room {nullptr};
    std::vector<std::string> messages {};
public:
    void say(const std::string& message); // public message to all
    void pm(const std::string& message, const std::string& receiver); // private message to one person 
    void receive(const std::string& message, const std::string& sender);
    bool operator==(const Person& other) { return name == other.name; }
    bool operator!=(const Person& other) { return name != other.name; }
    std::string getName() const { return name; }
    void setRoom(ChatRoom* r) { room = r; }

    Person(const std::string& name) : name(name) {}
};

class ChatRoom
{
    std::vector<Person*> members;
public:
    void broadcast(const std::string& sender, const std::string& message);
    void direct(const std::string& sender, const std::string& receiver, const std::string& message);
    void join(Person* person); 
    std::vector<Person*> getMembers() { return members; }
    Person* findPerson(const std::string other);
};

void Person::say(const std::string& message)
{
    room->broadcast(name, message);
}

void Person::pm(const std::string& message, const std::string& receiver)
{
    room->direct(name, receiver, message);
}

void Person::receive(const std::string& message, const std::string& sender)
{
    messages.push_back(sender + "said in pm: " + message);
    std::cout << name << " received from " << sender << ": " << message << std::endl;
}

void ChatRoom::broadcast(const std::string& sender, const std::string& message)
{
    for(const auto& m : members)
    {
        if(m->getName() != sender)
        {
            m->receive(message, sender);
        }
    }
}

void ChatRoom::direct(const std::string& sender, const std::string& receiver, const std::string& message)
{
    auto target = std::find_if(members.begin(), members.end(), 
        [&](const Person* p) { return p->getName() == receiver; } );

    if(target != members.end())
    {
        (*target)->receive(message, sender); 
    }
}

void ChatRoom::join(Person *person)
{
    members.push_back(person);
    broadcast(person->getName(), person->getName() + " joined chat");
    person->setRoom(this);
}

int main()
{
    ChatRoom room;
    Person john{"John"};
    room.join(&john); // here John doesn't receive a message about himself
    Person bill("Bill");
    room.join(&bill);
    john.say("Hi guys!");
    Person pete("Pete");
    room.join(&pete);
    pete.say("Hi all");
    pete.pm("Hope you're doing well", "John"); // only John received this message
}
