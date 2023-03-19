#include <iostream>
#include <string>
#include <vector>

class HtmlElement {
    friend class HtmlBuilder;
    std::string name, text;
    size_t indent = 3;
    std::vector<HtmlElement> elements;

    HtmlElement() {}

    HtmlElement(std::string name, std::string text) : name(name), text(text)
    {}

public:
    std::string str(int level = 0) const {
        std::string result;

        result += "\n" + std::string(indent * level, ' ') + "<" + name + ">";
        result += text;
        for(auto& e : elements) {
            result += e.str(level + 1);
        }
        result += "</" + name + ">";

        return result;
    }
};

class HtmlBuilder {
    HtmlElement root;

public:

    HtmlBuilder(std::string name) {
        root.name = name;
    }

    HtmlBuilder& addChild(std::string name, std::string text) {
        HtmlElement newElement{name, text};
        root.elements.emplace_back(newElement);
        return *this;
    }

    std::string str() const {
        return root.str();
    }

    static HtmlBuilder create(std::string name) {
        return {name};
    }

    operator HtmlElement() const {
        return root;
    }

    HtmlElement build() const {
        return root;
    }
};

int main() {

    //separate steps
    HtmlBuilder builder {"ul"};
    builder.addChild("li", "firstPoint");
    builder.addChild("li", "secondPoint");

    // fluent interface
    builder.addChild("li", "third").addChild("li", "fourth");

    std::cout << builder.str() << std::endl;

    auto el = HtmlBuilder::create("tr").build();
    std::cout << el.str() << std::endl;

    //build step by step
    HtmlElement el2 = HtmlBuilder::create("ul").addChild("ol", "testText").build();
    std::cout << el2.str() << std::endl;

}