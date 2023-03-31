#include <iostream>
#include <functional>

void SimpleFunction()
{
    std::cout  << "SimpleFunction\n";
}

int Addition(int a, int b)
{
    int c = a + b;
    std::cout << a << " + " << b << " = " << c << std::endl;
    return c;
}

double Average(int a, int b, int c)
{
    double d = (a + b + c) / 3.0;
    std::cout << "Average = " << d << std::endl;
    return d;
}

struct SimpleLogger
{
    std::function<void()> f;
    std::string name;

    SimpleLogger(const std::function<void()>& f, const std::string& name) : f(f), name(name) 
    {}

    void operator()() const
    {
        std::cout << name << " started\n";
        f();
        std::cout << name << " finished\n";
    }
};

template<typename Func>
struct MiddleLogger
{
    Func f;
    std::string name;

    MiddleLogger(const Func& f, const std::string& name) : f(f), name(name) 
    {}

    void operator()() const
    {
        std::cout << name << " started\n";
        f();
        std::cout << name << " finished\n";
    }
};

template<typename Func>
auto makeMiddleLogger(Func f, const std::string& name)
{
    return MiddleLogger<Func>{f, name};
}

template<typename> struct UniversalLogger;

template<typename R, typename... Args>
struct UniversalLogger<R(Args...)>
{
    std::function<R(Args...)> f;
    std::string name;

    UniversalLogger(const std::function<R(Args...)>& f, const std::string& name) : f(f), name(name) 
    {}

    R operator()(Args... args) 
    {
        std::cout << name << " started\n";
        R result = f(args...);
        std::cout << name << " finished\n";
        return result;
    }
};

template<typename R, typename... Args>
auto makeUniversalLogger(R(*func)(Args...), const std::string& name)
{
    return UniversalLogger<R(Args...)>
    (
        std::function<R(Args...)>(func),
        name
    );
}

int main()
{
    SimpleLogger(SimpleFunction, "SimpleFunction")();

    auto ml = makeMiddleLogger(SimpleFunction, "SimpleFunction");
    ml();

    auto ul = makeUniversalLogger(Addition, "Addition");
    ul(3, 7);

    auto ul2 = makeUniversalLogger(Average, "Average");
    ul2(5, 7, 9);
}