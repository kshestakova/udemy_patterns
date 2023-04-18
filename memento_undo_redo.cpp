#include <iostream>
#include <vector>
#include <memory>

class Memento
{
    int balance;
public:
    Memento(int b) : balance(b) {}

    friend class BankAccount;
};

class BankAccount
{
    int balance{0};
    std::vector<std::shared_ptr<Memento>> states;
    int stateNumber; // current state number in vector
public:
    BankAccount(int n) : balance(n)
    {
        auto m = std::make_shared<Memento>(balance);
        states.emplace_back(m);
        stateNumber = 0;
    }
    std::shared_ptr<Memento> deposit(int b) 
    {
        balance += b;
        auto m = std::make_shared<Memento>(balance);
        states.emplace_back(m);
        stateNumber++;
        return m; // automatically creates an object of return-type
    }

    void restore(const std::shared_ptr<Memento>& m) // restoring BankAccount object state from Memento
    {
        if(m)
        {
            balance = m->balance;
            auto t = std::make_shared<Memento>(balance);
            states.emplace_back(t);
            stateNumber++;
        }
    }

    std::shared_ptr<Memento> undo()
    {
        if(stateNumber > 0)
        {
            stateNumber--;
            auto m = states[stateNumber];
            balance = m->balance;
            return m;
        }
        else
        {
            std::cout << "Can't undo an operation" << std::endl;
            return {};
        }
    }

    std::shared_ptr<Memento> redo()
    {
        if(stateNumber < states.size() - 1)
        {
            stateNumber++;
            auto m = states[stateNumber];
            balance = m->balance;
            return m;
        }
        return {};
    }

    friend std::ostream& operator<<(std::ostream& os, const BankAccount& ba)
    {
        os << "Now balance is: " << ba.balance << std::endl;
        return os;
    }
};

int main()
{
    BankAccount account{100};
    std::cout << account;

    auto m1 = account.deposit(100);
    std::cout << account;
    auto m2 = account.deposit(150);
    std::cout << account;

    account.restore(m1);
    std::cout << account;
    account.restore(m2);
    std::cout << account;

    account.undo();
    std::cout << account;
    account.undo();
    std::cout << account;
    account.redo();
    std::cout << account;
}

