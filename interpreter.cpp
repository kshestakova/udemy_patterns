#include <iostream>
#include <vector>
#include <sstream>
#include <memory>

struct Token
{
    enum Type {integer, plus, minus, divider, multiplier, lparentheses, rparentheses} type;
    std::string text;

    Token(const Type& type, const std::string& text) : type(type), text(text) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& token)
    {
        os << "`" << token.text << "`  ";
        return os;
    }
};

std::vector<Token> lexing(std::string& input) 
{
    std::vector<Token> result;

    for(int i = 0; i < input.size(); ++i)
    {
        switch(input[i])
        {
            case '+':
                result.push_back(Token{Token::Type::plus, "+"});
                break;
            case '-':
                result.push_back(Token{Token::Type::minus, "-"});
                break;
            case '/':
                result.push_back(Token{Token::Type::divider, "/"});
                break;
            case '*':
                result.push_back(Token{Token::Type::multiplier, "*"});
                break;
            case '(':
                result.push_back(Token{Token::Type::lparentheses, "("});
                break;
            case ')':
                result.push_back(Token{Token::Type::rparentheses, ")"});
                break;
            default:
                std::ostringstream buffer;
                buffer << input[i];
                for(int j = i+1; i < input.size(); ++j)
                {
                    if(std::isdigit(input[j]))
                    {
                        buffer << input[j];
                        ++i;
                    }
                    else
                    {
                        result.push_back(Token{Token::Type::integer, buffer.str()});
                        break;
                    }
                }
                if (!buffer.str().empty()) 
                    result.push_back(Token{Token::integer, buffer.str()});
        }
    }

    return result;
}

struct Element
{
  virtual ~Element() = default;
  virtual int eval() const = 0;
};

struct Integer : Element
{
  int value;
  explicit Integer(const int value) : value(value) {}
  int eval() const override { return value; }
};

struct BinaryOperation : Element
{
  enum Type { addition, subtraction, multiplication, division } type;
  std::shared_ptr<Element> lhs, rhs;

  int eval() const override
  {
    switch(type)
    {
        case Type::addition: 
            return lhs->eval() + rhs->eval();
            break;
        case Type::subtraction: 
            return lhs->eval() - rhs->eval();
            break;
        case Type::multiplication: 
            return lhs->eval() * rhs->eval();
            break;
        case Type::division: 
            return lhs->eval() / rhs->eval();
            break;   
    }
  }
};

std::unique_ptr<Element> parse(const std::vector<Token>& tokens)
{
  auto result = std::make_unique<BinaryOperation>();
  bool have_lhs = false;

  for (size_t i = 0; i < tokens.size(); i++)
  {
    auto token = tokens[i];
    switch(token.type)
    {
    case Token::integer:
        {
            int value = stoi(token.text);
            auto integer = std::make_shared<Integer>(value);
            if (!have_lhs) {
                result->lhs = integer;
                have_lhs = true;
            }
            else result->rhs = integer;
        }
        break;
    case Token::plus: 
        result->type = BinaryOperation::addition;
        break;
    case Token::minus:
        result->type = BinaryOperation::subtraction;
        break;
    case Token::divider: 
        result->type = BinaryOperation::division;
        break;
    case Token::multiplier:
        result->type = BinaryOperation::multiplication;
        break;
    case Token::lparentheses: 
    {
        int j = i;
        for (; j < tokens.size(); ++j)
        if (tokens[j].type == Token::rparentheses)
            break; 

        std::vector<Token> subexpression(&tokens[i + 1], &tokens[j]);
        auto element = parse(subexpression);
        if (!have_lhs) 
        {
            result->lhs = move(element);
            have_lhs = true;
        }
        else result->rhs = move(element);

        i = j; 
    }
    break;
    }
  }
  return result;
}

int main()
{
    // now it works only for the most simple cases
    // investigation is in progress )) 
    std::string equation{"(85-13)+(6*2)"};
    auto tokens = lexing(equation);
    for(auto& i : tokens)
    {
        std::cout << i;
    }

    try 
    {
        auto parsed = parse(tokens);
        std::cout << equation << " = " << parsed->eval() << std::endl;
    } 
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
}