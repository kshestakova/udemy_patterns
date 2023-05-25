// visitor examples for design patterns c++ book
#include <sstream>
#include <string>
#include <iostream>

// definitions to use in visitor interface
struct SubtractionExpression;
struct DoubleExpression;
struct AdditionExpression;

// visitor interface - different methods (pure virtual for now)
// for different classes (hierarchy members)
struct ExpressionVisitor
{
    virtual void visit(DoubleExpression* de) = 0;
    virtual void visit(AdditionExpression* ae) = 0;
    virtual void visit(SubtractionExpression* se) = 0;
};

// real class with visitor interface 
// has working methods - overrides for different classes
// main goal - print objects of the different types in their own way
struct ExpressionPrinter : ExpressionVisitor
{
    std::ostringstream oss;
    std::string str() const { return oss.str(); }
    void visit(DoubleExpression* de) override;
    void visit(AdditionExpression* ae) override;
    void visit(SubtractionExpression* se) override;
};

// another class woth visitor interface
// helps to calculate the whole expression value 
struct ExpressionEvaluator : ExpressionVisitor
{
  double result;
  void visit(DoubleExpression* de) override;
  void visit(AdditionExpression* ae) override;
  void visit(SubtractionExpression* se) override;
};

// interface for example math expressions 
struct Expression
{
    // name choosen by accordance
    virtual void accept(ExpressionVisitor* visitor) = 0;
};

// single double value in the math expression
struct DoubleExpression : Expression
{
    double value;
    DoubleExpression(const double value)
        : value{ value } {}

    // functions in classes look the same
    // but they are different because of static (non-dynamic) typing in c++ 
    void accept(ExpressionVisitor* visitor) override
    {
        // calls visit() for DoubleExpression
        visitor->visit(this);
    }
};

// base addition in math expression - left + right
struct AdditionExpression : Expression
{
    // left and right could be single values or equations 
    Expression *left, *right;

    AdditionExpression(Expression* const left, Expression* const right)
        : left{ left }, right{ right } {}

    // don't forget to free memory allocated for pointers
    ~AdditionExpression()
    {
        delete left;
        delete right;
    }

    // the same, but different - calls visit() for AdditionExpression
    void accept(ExpressionVisitor* visitor) override
    {
        visitor->visit(this);
    }
};

// base subtraction in math expression - left - right
// looks like copypaste from the AdditionExpression, but for example it's OK
struct SubtractionExpression : Expression
{
    Expression *left, *right;

    SubtractionExpression(Expression* const left, Expression* const right)
        : left{ left }, right{ right } {}

    ~SubtractionExpression()
    {
        delete left;
        delete right;
    }

    // the same, but different - calls visit() for SubtractionExpression
    void accept(ExpressionVisitor* visitor) override
    {
        visitor->visit(this);
    }
};

// prints simple double value to console
void ExpressionPrinter::visit(DoubleExpression* de)
{
    oss << de->value;
}

// here we need braces only if addition is the right part of subtraction
// in other case we print a + b, not (a + b)
void ExpressionPrinter::visit(AdditionExpression* e)
{
    bool need_braces = dynamic_cast<SubtractionExpression*>(e->right);
    // calling accept() method, because a member of an expression also could be an expression
    e->left->accept(this);
    oss << "+";
    if (need_braces) oss << "(";
    e->right->accept(this);
    if (need_braces) oss << ")";
}

// here we need braces only if subtraction is the right part of addition
// in other case we print a - b, not (a - b)
void ExpressionPrinter::visit(SubtractionExpression* se)
{
    bool need_braces = dynamic_cast<SubtractionExpression*>(se->right);
    if (need_braces) oss << "(";
    se->left->accept(this);
    oss << "-";
    se->right->accept(this);
    if (need_braces) oss << ")";
}

// for expression a result = a
void ExpressionEvaluator::visit(DoubleExpression* de)
{
    result = de->value;
}

// for expression a + b result = a + b
void ExpressionEvaluator::visit(AdditionExpression* ae)
{
    // by calling accept() method we calculate a value of the left part
    ae->left->accept(this);
    auto temp = result;
    // then we do the same for the right part of expression
    ae->right->accept(this);
    result += temp;
}

// for expression a - b result = a - b
void ExpressionEvaluator::visit(SubtractionExpression* se)
{
    se->left->accept(this);
    auto temp = result;
    se->right->accept(this);
    temp -= result;
    result = temp;
}

int main()
{
    // 1 + (2 - 3) - an example with a simple double, a subtraction and an addition 
    auto e = new AdditionExpression{
        new DoubleExpression{ 1 },
        new SubtractionExpression {
            new DoubleExpression{ 2 },
            new DoubleExpression{ 3 }
        }
    };
    // stream to print to console
    std::ostringstream oss;
    // visitors to print and to calculate a value
    ExpressionPrinter printer;
    ExpressionEvaluator evaluator;
    // printing an expression by visitor pattern
    printer.visit(e);
    // calculating its value by visitor pattern
    evaluator.visit(e);
    std::cout << printer.str() << " = " << evaluator.result << std::endl;
}
