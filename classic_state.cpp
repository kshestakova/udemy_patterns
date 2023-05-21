#include <iostream>

struct LightSwitch;

struct State
{
    // functions called only in case if there are no overrides
    virtual void on(LightSwitch* ls) 
    {
        std::cout << "Light is alreary on\n";
    }
    virtual void off(LightSwitch* ls) 
    {
        std::cout << "Light is alreary off\n";
    }
};

struct OffState;


struct OnState : State
{
    OnState()
    {
        std::cout << "Light turned on\n";
    }

    // void on(LightSwitch* ls) from parent to say that state is on already

    void off(LightSwitch* ls) override;
};

struct OffState : State
{
    OffState()
    {
        std::cout << "Light turned off\n";
    }

    // void off(LightSwitch* ls) from parent to say that state is off already

    void on(LightSwitch* ls) override;
};

struct LightSwitch
{
    State *state;

    LightSwitch()
    {
        // light is off by default
        state = new OffState();
    }

    void setState(State* s)
    {
        state = s;
    }

    void on() 
    { 
        state->on(this); 
    }

    void off() 
    {
        state->off(this); 
    }
};

void OnState::off(LightSwitch* ls) 
{
    std::cout << "Switching light off\n";
    ls->setState(new OffState());
    delete this;
}

void OffState::on(LightSwitch* ls)
{
    std::cout << "Switching light on\n";
    ls->setState(new OnState());
    delete this;
}

int main()
{
    LightSwitch ls;
    ls.on();   
    ls.off();
    ls.off();
    ls.on();
    ls.on();
}