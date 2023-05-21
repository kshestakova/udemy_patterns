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

template<typename T>
struct TypeProcessor
{
    void appendItems(const std::vector<std::string>& items)
    {
        myST.start(oss);
        for(auto& i : items)
        {
            myST.addItem(oss, i);
        }
        myST.end(oss);
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
    T myST;
    std::ostringstream oss;
};

int main()
{
    std::vector<std::string> strings {"one", "two", "three", "four", "five"};
    
    TypeProcessor<HtmlStrategy> htmlTP;
    htmlTP.appendItems(strings);
    std::cout << htmlTP.str();

    TypeProcessor<MarkdownStrategy> markdownTP;    
    markdownTP.appendItems(strings);
    std::cout << markdownTP.str();

}