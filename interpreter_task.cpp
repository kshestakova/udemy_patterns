#include <iostream>
#include <vector>
#include <map>

/*
You are asked to write an expression processor for simple numeric expressions with the following constraints:

    Expressions use integral values (e.g., "13" ), single-letter variables defined in variables , 
    as well as + and - operators only
    There is no need to support braces or any other operations
    If a variable is not found in Variables  (or if we encounter a variable with >1 letter, e.g. ab),
    the evaluator returns 0 (zero)
    In case of any parsing failure, evaluator returns 0

Example:

    calculate("1+2+3")  should return 6
    calculate("1+2+xy")  should return 0
    calculate("10-2-x")  when x=3 is in variables should return 5
*/

struct ExpressionProcessor
{
    struct Token
    {
        enum Type {plus, minus, integer, variable} type;
        std::string text;
        
        Token(const Type& type, const std::string& text) : type(type), text(text) {}
    };

    std::map<char, int> variables;
    std::vector<Token> tokens;

    void tokenize(const std::string& expression);
    int calculate(const std::string& expression);
};

typedef ExpressionProcessor::Token Token;

void ExpressionProcessor::tokenize(const std::string& expression)
{
    tokens.clear();
    for(int i = 0; i < expression.size(); ++i) 
    {
        switch(expression[i])
        {
            case '+':
                tokens.emplace_back(Token{Token::Type::plus, "+"});
                break;
            case '-':
                tokens.emplace_back(Token{Token::Type::minus, "-"});
                break;
            default:
                if(std::isdigit(expression[i]))
                {
                    std::string s{""};
                    s += expression[i];
                    for(int j = i+1; j < expression.size(); ++j)
                    {
                        if(std::isdigit(expression[j]))
                        {
                            s += expression[j];
                            ++i;
                        }
                        else
                        {
                            tokens.push_back(Token{Token::Type::integer, s});
                            s = "";
                            break;
                        }
                    }
                    if (s.size() != 0) 
                    {
                        tokens.push_back(Token{Token::Type::integer, s});
                    }
                    break;                        
                }
                else
                {
                    std::string s{""};
                    s += expression[i];
                    for(int j = i+1; j < expression.size(); ++j)
                    {
                        if(std::isalpha(expression[j]))
                        {
                            s += expression[j];
                            ++i;
                        }
                        else
                        {
                            tokens.push_back(Token{Token::Type::variable, s});
                            s = "";
                            break;
                        }
                    }
                    if (s.size() != 0) 
                        tokens.push_back(Token{Token::Type::variable, s});
                    break;  
                }
        }
    }
}

int ExpressionProcessor::calculate(const std::string& expression)
  {
    tokenize(expression);
    std::vector<int> numbers;
    int result = 0;
    
    for(int i = 0; i < tokens.size(); i++)
    {
        switch(tokens[i].type)
        {
            case Token::Type::variable:
                if(tokens[i].text.size() == 1 && variables.find(tokens[i].text[0]) != variables.end())
                {
                    numbers.emplace_back(variables[tokens[i].text[0]]);
                    break;
                }
                else
                {
                    return 0;
                }
            case Token::Type::integer:
                try
                {
                    numbers.emplace_back(std::stoi(tokens[i].text));
                    break;
                }
                catch(std::exception& e)
                {
                    std::cout << e.what();
                    return 0;
                }
            case Token::Type::plus:
            case Token::Type::minus:
                numbers.emplace_back(0);
                break;
        }
    }

    result += numbers[0];

    for(int i = 1; i < tokens.size(); i++)
    {
        switch(tokens[i].type)
        {
            case Token::Type::plus:
                if(i < tokens.size() - 1)
                {
                    result += numbers[i+1];
                    i++;
                }
                break;
            case Token::Type::minus:
                if(i < tokens.size() - 1)
                {
                    result -= numbers[i+1];
                    i++;
                }
                break;
            case Token::Type::integer:
            case Token::Type::variable:
                i++;
                break;
        }
    }

    return result;
}

int main()
{
    ExpressionProcessor ep;
    ep.variables['x'] = 3;
    std::string equation = "1+2+3";
    std::cout << equation << " = " << ep.calculate(equation) << std::endl;

    equation = "1+2+xy";
    std::cout << equation << " = " << ep.calculate(equation) << std::endl;

    equation = "10-2-x";
    std::cout << equation << " = " << ep.calculate(equation) << std::endl;

}