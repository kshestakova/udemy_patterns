#include <iostream>

struct Pingable
{
    virtual std::string ping(const std::string& message) const = 0;
    virtual ~Pingable() = default;
};

struct Pong : Pingable
{
    std::string ping(const std::string& message) const override
    {
        // here we can have some logic 
        // which runs on server
        return message + " pong\n";
    }
};

void tryPing(Pingable& p)
{
    std::cout << p.ping("ping");
}

int main()
{
    Pong p;
    for(int i = 0; i < 10; ++i)
    {
        tryPing(p);
    }
}