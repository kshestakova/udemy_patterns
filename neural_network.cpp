#include <iostream>
#include <vector>

struct Neuron;

// interface for handling both Neurons and NeuronLayers

template<typename Self>
struct SomeNeurons
{
    template<typename T>
    void connectTo(T& other);
    /* template<typename T>
    void connectTo(T& other)
    {
        for(Neuron& from : *static_cast<Self*>(this))
        {
            for(Neuron& to : other)
            {
                from.out.push_back(&to);
                to.in.push_back(&from);
            }
        }
    }*/

};

template <typename Self>
template <typename T>
void SomeNeurons<Self>::connectTo(T& other) {
    Self *This = static_cast<Self*>(this);
 
    for (Neuron& from : *This) {
        for (Neuron& to : other) {
            from.out.push_back(&to);
            to.in.push_back(&from);
        }
    }
}

struct Neuron : SomeNeurons<Neuron>
{
    std::vector<Neuron*> in, out;
    unsigned int id;

    Neuron()
    {
        static int n = 1;
        id = n++;
    }

    Neuron* begin() {return this;}
    Neuron* end() {return this + 1;}

    friend std::ostream& operator <<(std::ostream& os, const Neuron& obj)
    {
        for(Neuron* n : obj.in)
        {
            os << n->id << " --> " << obj.id << std::endl;
        }
        for(Neuron* n : obj.out)
        {
            os << obj.id << " --> " << n->id << std::endl;
        }
        return os;
    }
};

// a little issue here - std::vector doesn't have virtual destructor
// that's why it's better not to inherit from it 
// but use boost::ptr_vector or other with virtual destructor
// anyway it's just a prototype of CRTP - curiously recurring template pattern 

struct NeuronLayer : std::vector<Neuron>, SomeNeurons<Neuron>
{
    NeuronLayer(int c)
    {
        while(c-- > 0)
        {
            emplace_back(Neuron{});
        }
    }

    friend std::ostream& operator <<(std::ostream& os, NeuronLayer& obj)
    {
        for(auto& n : obj)
        {
            os << n << "\t";
        }
        return os;
    }
};



int main()
{
    Neuron n1, n2, n3;
    n1.connectTo(n2);
    n2.connectTo(n3);
    n3.connectTo(n1);

    std::cout << n1 << n2 << n3;

    NeuronLayer l1 {5};
    l1.connectTo(n1);
    
    std::cout << "Neuron n1: " << n1.id << std::endl << n1 << std::endl;
    std::cout << "Layer l1: " << std::endl << l1 << std::endl;

    NeuronLayer l2{2}, l3{3};
    l2.connectTo(l3);

    std::cout << "Layer l2" << std::endl << l2;
    std::cout << "Layer l3" << std::endl << l3;
}