#include <iostream>
#include <string>
#include <boost/signals2.hpp>

using namespace boost::signals2;

struct Query
{
  std::string name;
  enum Argument { attack, defense } argument;
  int result;


  Query(const std::string& name, const Argument argument, const int result)
    : name(name), argument(argument), result(result)
  {
  }
};

struct Game 
{
  signal<void(Query&)> queries;
};

class Creature
{
  Game& game;
  int attack, defense;

public:
  std::string name;
  Creature(Game& game, const std::string& name, const int attack, const int defense)
    : game(game), attack(attack), defense(defense), name(name)
  {
  }
  
  int getAttack() const
  {
    Query q{ name, Query::Argument::attack, attack };
    game.queries(q);
    return q.result;
  }

  friend std::ostream& operator<<(std::ostream& os, const Creature& c)
  {
    return os << "name: " << c.name << " attack: " << c.getAttack() << " defense: " << c.defense;
  }
};

class CreatureModifier
{
  Game& game;
  Creature& creature;
public:
  virtual ~CreatureModifier() = default;

  CreatureModifier(Game& game, Creature& creature)
    : game(game), creature(creature)
  {
  }
};

class DoubleAttackModifier : public CreatureModifier
{
  connection cn;
public:
  DoubleAttackModifier(Game& game, Creature& creature) : CreatureModifier(game, creature)
  {

    conn = game.queries.connect([&](Query& q)
    {
      if (q.creature_name == creature.name && 
          q.argument == Query::Argument::attack)
          q.result *= 2;
    });
  }

  ~DoubleAttackModifier()
  {
    cn.disconnect();
  }
};

// realization with query
int main(int ac, char* av)
{
  Game game;
  Creature goblin{ game, "My first goblin", 4, 5 };

  std::cout << goblin;

  {
    DoubleAttackModifier dam{ game, goblin };
    std::cout << goblin ;
  } // when leaving block, modifier stopped working

  std::cout << goblin;

  getchar();
  return 0;
}
