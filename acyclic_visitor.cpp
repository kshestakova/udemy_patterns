#include <iostream>
#include <string>
#include <sstream>

// classic (cyclic) visitor: 
// function overloading
// works only on a stable hierarchy 
// (it could be tough to delete some types or add types out of hierarchy )
//
// acyclic visitor: based on RTTI paradigm (run-time type information)
// works a little bit slower, but has no hierarchy limitations

template <typename Visitable>
struct Visitor
{
  virtual void visit(Visitable& obj) = 0;
};

// marker interface - shows that object 
// could be possibly used with visitor pattern
struct VisitorBase 
{
  virtual ~VisitorBase() = default;
};

// interface implements base visitor accept() method logic
// object is acceptable only if it has type Visitor<Expression>
// that is checked by dynamic_cast
struct Expression
{
  virtual ~Expression() = default;

  virtual void accept(VisitorBase& obj)
  {
    using EV = Visitor<Expression>;
    if (auto ev = dynamic_cast<EV*>(&obj))
      ev->visit(*this);
  }
};

// expression with only pure double number
struct DoubleExpression : Expression
{
  double value;

  DoubleExpression(double value) : value(value) {}

// the same logic - object is acceptable only with his own type - Visitor<DoubleExpression>
// else visit() doesn't work here
  virtual void accept(VisitorBase& obj)
  {
    using DEV = Visitor<DoubleExpression>;
    if (auto ev = dynamic_cast<DEV*>(&obj))
      ev->visit(*this);
  }
};

// the same realization for (a+b) expressions
// a and b coul be double values, additional expressions etc. 
struct AdditionExpression : Expression
{
  Expression *left, *right;

  AdditionExpression(Expression *left, Expression *right) : left(left), right(right) {}

  ~AdditionExpression()
  {
    delete left;
    delete right;
  }

  virtual void accept(VisitorBase& obj)
  {
    using AEV = Visitor<AdditionExpression>;
    if (auto ev = dynamic_cast<AEV*>(&obj))
      ev->visit(*this);
  }
};

// marked by VisitorBase type
// which can work with visitors for Expression, DoubleExpression and AdditionExpression objects
struct ExpressionPrinter : VisitorBase,
                           Visitor<Expression>,
                           Visitor<DoubleExpression>,
                           Visitor<AdditionExpression>
{

// different visit() realizations for different (inherited) types
  void visit(Expression &obj) override
  {
    // doing nothing for general type
  }

// we can remove double visitor without failure
// in that case we only don't see doubles in the final printed expression
  void visit(DoubleExpression &obj) override
  {
    oss << obj.value;
  }

  void visit(AdditionExpression &obj) override
  {
    oss << "(";
    obj.left->accept(*this);
    oss << "+";
    obj.right->accept(*this);
    oss << ")";
  }

  std::string str() const { return oss.str(); }
private:
  std::ostringstream oss;
};

int main()
{
  auto e = new AdditionExpression{
    new DoubleExpression{1},
    new AdditionExpression{
      new DoubleExpression{2},
      new DoubleExpression{3}
    }
  };

  ExpressionPrinter ep;
  ep.visit(*e);
  std::cout << ep.str() << "\n";

	return 0;
}
