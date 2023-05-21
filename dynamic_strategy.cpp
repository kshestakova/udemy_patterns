#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

enum class StrategyType
{
    markdown,
    html
};

struct BaseStrategy
{
    virtual void start(std::ostringstream& oss) {}
    virtual void end(std::ostringstream& oss) {}
    virtual void addItem(std::ostringstream& oss, const std::string& item) = 0;
};

struct MarkdownStrategy : BaseStrategy
{
    void addItem(std::ostringstream& oss, const std::string& item) override
    {
        oss << "* " << item << std::endl;
    }
};

struct HtmlStrategy : BaseStrategy
{
    void addItem(std::ostringstream& oss, const std::string& item) override
    {
        oss << "\t<li>" << item << "</li>\n";
    }

    void start(std::ostringstream& oss) override
    {
        oss << "<ul>\n";
    }

    void end(std::ostringstream& oss) override
    {
        oss << "</ul>\n";
    }
};

struct TypeProcessor
{
    void setStrategy(const StrategyType& st)
    {
        switch (st)
        {
        case StrategyType::html:
            myST = std::make_unique<HtmlStrategy>();
            break;
        
        case StrategyType::markdown:
            myST = std::make_unique<MarkdownStrategy>();
            break;

        default:
            break;
        }
    }

    void appendItems(const std::vector<std::string>& items)
    {
        myST->start(oss);
        for(auto& i : items)
        {
            myST->addItem(oss, i);
        }
        myST->end(oss);
    }

    void clear()
    {
        oss.str("");
        oss.clear();
    }

    std::string str()
    {
        return oss.str();
    }

private:
    std::unique_ptr<BaseStrategy> myST;
    std::ostringstream oss;
};

int main()
{
    std::vector<std::string> strings {"one", "two", "three", "four", "five"};
    
    TypeProcessor tp;
    tp.setStrategy(StrategyType::html);
    tp.appendItems(strings);
    std::cout << tp.str();
    
    tp.clear();
    tp.setStrategy(StrategyType::markdown);
    tp.appendItems(strings);
    std::cout << tp.str();

}