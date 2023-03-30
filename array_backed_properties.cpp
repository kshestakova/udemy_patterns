#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>

class Creature
{
    enum AbilityTypes {strenght, intelligence, mana, count};
    std::array<int, count> abilities;
public:
    int getStrength() {return abilities[strenght];}
    int getIntelligence() {return abilities[intelligence];}
    int getMana() {return abilities[mana];}

    void setStrength(const int value) {abilities[strenght] = value;}
    void setIntelligence(const int value) {abilities[intelligence] = value;}
    void setMana(const int value) {abilities[mana] = value;} 

    int getSum(){return std::accumulate(abilities.begin(), abilities.end(), 1);}
    double getAverage(){return getSum()/(double)count;}
    int getMax(){return *std::max_element(abilities.begin(), abilities.end());} 
};

int main()
{
    Creature c;
    c.setIntelligence(10);
    c.setMana(15);
    c.setStrength(6);

    std::cout << "Creature has " << c.getSum() << " points.\n Maximum is "
        << c.getMax() << " and average is " << c.getAverage() << std::endl;
}