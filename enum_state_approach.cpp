#include <iostream>
#include <map>
#include <vector>

enum class State
{
    staying,
    stand_up,
    sitting,
    sleeping, 
    fighting,
    won,
    running,
    walking,
    dead
};

std::string printState(State s)
{
    switch(s)
    {
        case State::staying:
            return "Staying"; 
            break;
        case State::stand_up:
            return "Stand_up"; 
            break;
        case State::sitting:
            return "Sitting"; 
            break;
        case State::sleeping:
            return "Sleeping"; 
            break;
        case State::fighting:
            return "Fighting"; 
            break;
        case State::won:
            return "Won"; 
            break;
        case State::running:
            return "Running"; 
            break;
        case State::walking:
            return "Walking"; 
            break;
        case State::dead:
            return "Dead"; 
            break;
        default:
            return "No such state"; 
    }
}

enum class Trigger
{
    enemy_comes,
    received_quest,
    got_tired,
    got_bored,
    got_attacked,
    got_overfed,
    got_killed
};

std::string printTrigger(Trigger t)
{
    switch(t)
    {
        case Trigger::enemy_comes:
            return "Enemy comes";
            break;
        case Trigger::received_quest:
            return "Received quest";
            break;
        case Trigger::got_tired:
            return "Got tired";
            break;
        case Trigger::got_bored:
            return "God bored";
            break;
        case Trigger::got_attacked:
            return "Got attacked";
            break;
        case Trigger::got_overfed:
            return "Got overfed";
            break;
        case Trigger::got_killed:
            return "Got killed";
            break;
        default:
            return "No such trigger";
    }
}

int main()
{
    std::map<State, std::vector<std::pair<Trigger, State>>> stateMachine;

    stateMachine[State::staying] = {
        {Trigger::enemy_comes, State::stand_up},
        {Trigger::received_quest, State::walking},
        {Trigger::got_tired, State::sitting},
        {Trigger::got_bored, State::sleeping},
        };

    stateMachine[State::stand_up] = {
        {Trigger::got_attacked, State::fighting},
        {Trigger::received_quest, State::running}
        };

    stateMachine[State::sitting] = {
        {Trigger::got_overfed, State::walking},
        {Trigger::received_quest, State::stand_up},
        {Trigger::got_tired, State::sleeping},
        {Trigger::got_bored, State::running}
        };
    
    stateMachine[State::sleeping] = {
        {Trigger::enemy_comes, State::fighting},
        {Trigger::received_quest, State::stand_up},
        {Trigger::got_bored, State::stand_up}
        };
        
    stateMachine[State::fighting] = {
        {Trigger::got_killed, State::dead},
        {Trigger::got_bored, State::won}
        };
    
    stateMachine[State::won] = {
        {Trigger::enemy_comes, State::fighting},
        {Trigger::received_quest, State::stand_up}
        };

    stateMachine[State::running] = {
        {Trigger::got_tired, State::sitting},
        {Trigger::got_bored, State::staying}
        };

    stateMachine[State::walking] = {
        {Trigger::got_tired, State::sitting},
        {Trigger::got_bored, State::stand_up},
        {Trigger::received_quest, State::running}
        };

    State current = State::staying, finish = State::dead;

    while(true)
    {
        std::cout << "Now you are " << printState(current) << ". What trigger will be the next?\n";

        int i = 0;
        for(auto& t : stateMachine[current])
        {
            std::cout << i++ << ". " << printTrigger(t.first) << " --> " << printState(t.second) << std::endl;
        }

        int choice;
        std::cin >> choice; 

        if(choice < 0 || choice >= stateMachine[current].size()) 
        {
            std::cout << "Let's try again\n";
        }
        else
        {
            current = stateMachine[current][choice].second; 
            if(current == finish) 
            {
                std::cout << "End state reached. Bye!\n";
                break;
            }
        }
    }
}