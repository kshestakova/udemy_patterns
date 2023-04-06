#include <iostream>

struct Drawable
{
    virtual void draw() = 0;
    virtual ~Drawable() = default;
};


// just usual class: object fully initialized on construction
class Picture : Drawable
{
    std::string filename;
public:
    Picture(const std::string& filename) : filename(filename) {}
    void draw() override
    {
        std::cout << "Imagine that I'm drawing a picture: " << filename << "\n";
    }
};

// lazy initialization in proxy class
class LazyPicture : public Drawable
{
    
    Picture* p{nullptr};
    std::string filename;
public:
    // at first we use a nullptr-object Picture
    LazyPicture(const std::string& filename) : filename(filename) {}

    // and only when we need to draw it we initialize it 
    // and, for example, some content we need to load
    void draw()
    {
        if(!p)
        {
            p = new Picture(filename);
            p->draw();
        }
    }
    ~LazyPicture() { delete p; }
};

int main()
{
    Picture p{"kittens.jpg"};
    p.draw();

    LazyPicture lp{"puppies.png"};
    lp.draw();
}