#include <iostream>

template<typename T>
struct BinaryTree;

template<typename T>
struct Node
{
    T value = T();
    Node<T>* left{nullptr};
    Node<T>* right{nullptr};
    Node<T>* parent{nullptr};
    BinaryTree<T>* tree{nullptr};

    Node(T value): value(value) {}
    Node(T value, Node<T>* left, Node<T>* right) :
        value(value), left(left), right(right)
    {
        this->left->parent = this->right->parent = this;
        this->left->tree = this->right->tree = tree;
    }

    void setTree(BinaryTree<T>* t)
    {
        tree = t;
        if(left) left->setTree(t);
        if(right) right->setTree(t);
    }

    ~Node() 
    {
        if(left) delete left;
        if(right) delete right;
    }
};

template<typename T>
struct BinaryTree
{
    Node<T>* root{nullptr};

    BinaryTree<T>(Node<T>* root) : root(root)
    {
        root->setTree(this);
    }

    ~BinaryTree() 
    {
        if(root) delete root;
    }

    template<typename U>
    struct PreOrderIterator
    {
        Node<U>* current;

        PreOrderIterator(Node<U>* c) : current(c) {}

        PreOrderIterator<U>& operator++()
        {
            // going down to the left
            if(current->right)
            {
                current = current->right;
                while(current->left) current = current->left;
            }
            else
            {
                // going up from the right
                Node<T>* p = current->parent;
                while(p && current == p->right)
                {
                    current = p;
                    p = p->parent;
                } 
                current = p;
            }
            return *this;
        }

        Node<U>& operator*() { return *current; }
        bool operator!=(const PreOrderIterator<U>& c) { return c.current != current; }
    };

    typedef PreOrderIterator<T> iterator;

    iterator begin()
    {
        Node<T>* n = root;
        if(n)
        {
            while(n->left) n = n->left;
        }
        return iterator{ n }; 
    }

    iterator end()
    {
        return iterator{nullptr};
    }
};

int main()
{
    /*
                            4 me
                     /               \
              2  my mother        5 my father
            /               \
    1 my mother's mother   3 my mother's father      
    */

    BinaryTree<std::string> familyTree {
        new Node<std::string>{"me", 
            new Node<std::string>{"my mother", 
                new Node<std::string>{"my mother's mother"},
                new Node<std::string>{"my mother's father"} },
            new Node<std::string>{"my father"}
        }
    };

    for(auto it = familyTree.begin(); it != familyTree.end(); ++it) 
    {
        std::cout << (*it).value << std::endl;
    }
}