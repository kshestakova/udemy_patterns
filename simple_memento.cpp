#include <iostream>

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
public:
    Memento deposit(int b) 
    {
        balance += b;
        return {balance}; // automatically creates an object of return-type
    }

    void restore(const Memento& m) // restoring BankAccount object state from Memento
    {
        balance = m.balance;
    }

    friend std::ostream& operator<<(std::ostream& os, const BankAccount& ba)
    {
        os << "Now balance is: " << ba.balance << std::endl;
        return os;
    }
};

int main()
{
    BankAccount account;
    std::cout << account;

    auto m1 = account.deposit(100);
    std::cout << account;
    auto m2 = account.deposit(150);
    std::cout << account;

    account.restore(m1);
    std::cout << account;
    account.restore(m2);
    std::cout << account;
}

