#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Diary
{
    std::string name;
    std::vector<std::string> entries;

    Diary(const std::string &name) : name(name) {}

    void addEntity(const std::string& newEntry)
    {
        entries.push_back(newEntry);
    }
};

struct PersistencyManager
{
    void saveToFile(const Diary &data, const std::string& filename)
    {
        std::ofstream ofs(filename);
        ofs << data.name << std::endl;
        for(auto& e : data.entries)
        {
            ofs << e << std::endl;
        }
        ofs.close();
    }

    Diary loadFromFile(const std::string& filename)
    {
        std::ifstream ifs(filename);
        std::string line;

        std::getline(ifs, line);
        Diary loadedDiary(line);

        while(ifs.good()){
            std::getline(ifs,line);
            if(line.size())
                loadedDiary.addEntity(line);
        }
        ifs.close();

        return loadedDiary;
    }
};

int main() {

    Diary myDiary("Black diary");
    myDiary.addEntity("Today I started the Udemy course");
    myDiary.addEntity("Today I learned two enterprise patterns");

    for(auto& e : myDiary.entries) std::cout << e << std::endl;

    PersistencyManager pm;
    pm.saveToFile(myDiary, "diary.txt");

    Diary yourDiary {pm.loadFromFile("diary.txt")};

    for(auto& e : yourDiary.entries) std::cout << e << std::endl;

    return 0;
}
