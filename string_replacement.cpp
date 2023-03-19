#include <iostream>
#include <cctype>

void punctuationReplacement(std::string& s)
{
    for(int i = 0; i < s.size(); ++i)
    {
        if(ispunct(s[i])) s[i] = '!';
    }
}

int main()
{
    std::string testString = "To be, or not to be, that is the question:";
    std::cout << testString << std::endl;
    punctuationReplacement(testString);
    std::cout << testString << std::endl;
}