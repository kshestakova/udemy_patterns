#include <iostream>

// we have two shape types - squares and circles
// we want to draw them in raster or vector format

// we use bridge pattern to separate interface and implementations
// we will have general interface for rendering circles and squares
// and concrete vector and raster renderers which implement this interface

class Renderer
{
public:
    virtual void renderCircle(int x, int y, double radius) = 0;
    virtual void renderSquare(int x, int y, double side) = 0;
};

class RasterRenderer : public Renderer
{
public:
    void renderCircle(int x, int y, double radius) override
    {
        std::cout << "Rendering raster circle with radius " << radius << std::endl;
    }
    void renderSquare(int x, int y, double side) 
    {
        std::cout << "Rendering raster square with side length " << side << std::endl;
    }
};

class VectorRenderer : public Renderer
{
public:
    void renderCircle(int x, int y, double radius) override
    {
        std::cout << "Rendering vector circle with radius " << radius << std::endl;
    }
    void renderSquare(int x, int y, double side) 
    {
        std::cout << "Rendering vector square with side length " << side << std::endl;
    }
};

class Shape
{
public:
    Renderer& renderer;
    Shape(Renderer& renderer) : renderer{renderer} {};
    ~Shape() = default;
    virtual void draw() = 0;
    virtual void resize(double d) = 0;
};

class Circle : public Shape
{
    int x, y;
    double radius;
public:
    Circle(Renderer& renderer, int x, int y, double radius) : 
            Shape{renderer}, x(x), y(y), radius(radius)
    {
        std::cout << "Creating circle with radius " << radius << std::endl;
    }
    void draw() override
    {
        renderer.renderCircle(x, y, radius);
    };
    void resize(double d) override
    {
        radius *= d;
    }
};

class Square : public Shape
{
    int x, y;
    double side;
public:
    Square(Renderer& renderer, int x, int y, double side) : 
            Shape{renderer}, x(x), y(y), side(side)
    {
        std::cout << "Creating square\n";
    }
    void draw() override
    {
        renderer.renderSquare(x, y, side);
    };
    void resize(double d) override
    {
        side += d;
    }
};

int main()
{
    RasterRenderer rr;
    VectorRenderer vr;

    Circle rc(rr, 1, 2, 3);
    rc.draw();
    rc.resize(3);
    rc.draw(); 

    Circle vc(vr, 3, 4, 2);
    vc.draw();
    vc.resize(2);
    vc.draw(); 

    Square rs(rr, 0, 0, 5);
    rs.draw();
    rs.resize(5);
    rs.draw(); 

    Square vs(vr, 3, 4, 10);
    vs.draw();
    vs.resize(4);
    vs.draw(); 
}