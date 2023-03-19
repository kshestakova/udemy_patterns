#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

enum class CoordinateType
{
    carthesian,
    polar
};

class Point 
{
    Point(float a, float b) : x(a), y(b) {}
public:
    float x;
    float y;

    static Point NewCarthesian(float a, float b) 
    {
        return {a, b};
    }

    static Point NewPolar(float a, float b) 
    {
        return {a * cos(b), a * sin(b)};
    }

    friend std::ostream& operator<<(std::ostream& out, Point& p)
    {
        out << p.x << ", " << p.y << std::endl;
        return out;
    }
};

int main()
{
    // Polar x {1, 1}; -- inappropriate because constructor is private

    //  instead of constructor use factory methods with same arguments 
    // and create different objects with specific parameters
    
    Point one = Point::NewCarthesian(1, M_PI_4);
    Point two = Point::NewPolar(1, M_PI_4);

    std::cout << one;
    std::cout << two;
}