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
    
    float x;
    float y;

    class PointFactory 
    {
        PointFactory() {}
    public:
        // inner class has access to Point(float, float) private constructor
        static Point NewCarthesian(float a, float b) 
        {
            return {a, b};
        }

        static Point NewPolar(float a, float b) 
        {
            return {a * cos(b), a * sin(b)};
        }
    };

public:
    // public member to use inner class method
    // can be private with public getter
    static PointFactory Factory;

    friend std::ostream& operator<<(std::ostream& out, Point& p)
    {
        out << p.x << ", " << p.y << std::endl;
        return out;
    }
};

int main()
{
    // creating objects via inner factory and its public methods

    Point one = Point::Factory.NewCarthesian(1, M_PI_4);
    Point two = Point::Factory.NewPolar(1, M_PI_4);

    std::cout << one;
    std::cout << two;
}