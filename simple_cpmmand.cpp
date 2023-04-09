#include <iostream>
#include <vector>

struct BankAccount
{
    int balance{0};
    int overdraftLimit{-500};

    void deposit(int amount) { balance += amount; std::cout << *this; }
    bool withdraw(int amount) 
    { 
        if(balance - overdraftLimit >= amount) 
        {
            balance -= amount; 
            std::cout << *this;
            return true;
        } 
        else
        {
            std::cout << "Withdrawal " << amount << " restricted\n";
            return false;
        }
    }

    friend std::ostream& operator<<(std::ostream &os, const BankAccount& ba) 
    {
        os << "Balance: " << ba.balance << ", overdraftLimit: " << ba.overdraftLimit << std::endl;
        return os; 
    }
};

class Command
{
public:
    virtual void call() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

class BankAccountCommand : public Command
{
    BankAccount& account;
    int amount;

public:
    enum Actions {deposit, withdraw} action;
    bool succeeded;
    
    BankAccountCommand(BankAccount& account, Actions action, int amount) : 
        account(account), action(action), amount(amount), succeeded(false) {}

    void call()
    {
        switch(action)
        {
            case Actions::deposit:
                succeeded = true;
                account.deposit(amount);
                break;
            case Actions::withdraw:
                succeeded = account.withdraw(amount);
                break;     
        }
    }

    void undo()
    {
        if(!succeeded) return;

        switch(action)
        {
            case Actions::withdraw:
                account.deposit(amount);
                break;
            case Actions::deposit:
                account.withdraw(amount);
                break;     
        }
    }

    ~BankAccountCommand() = default;
};

// let's use std::vector here, but keep in mind that it has only virtual destructor
class CompositeBankAccountCommand : public std::vector<BankAccountCommand>, Command
{
public:
  CompositeBankAccountCommand(const std::initializer_list<value_type>& commands)
    : vector<BankAccountCommand>(commands)
  {
  }

  void call() override
  {
    for (auto& cmd : *this)
      cmd.call();
  }

  void undo() override
  {
    for (auto it = rbegin(); it != rend(); ++it)
      it->undo();
  }

  ~CompositeBankAccountCommand() = default;
};

// if one command fails, all next commands fail too
class DependentCompositeCommand : public CompositeBankAccountCommand
{
public:
  explicit DependentCompositeCommand(
    const std::initializer_list<value_type>& commands)
    : CompositeBankAccountCommand{ commands } {}

  void call() override
  {
    bool ok = true;
    for (auto& cmd : *this)
    {
      if (ok)
      {
        cmd.call();
        ok = cmd.succeeded;
      }
      else
      {
        cmd.succeeded = false;
      }
    }
  }

  ~DependentCompositeCommand() = default;
};

class MoneyTransferCommand : public DependentCompositeCommand
{
public:
  MoneyTransferCommand(BankAccount& from,
    BankAccount& to, int amount): 
    DependentCompositeCommand
    {
      BankAccountCommand{from, BankAccountCommand::withdraw, amount},
      BankAccountCommand{to, BankAccountCommand::deposit, amount}
    } {}

    ~MoneyTransferCommand() = default;
};

int main()
{
    // simple approach
    BankAccount ba;
    std::vector<BankAccountCommand> bacv{
        BankAccountCommand{ba, BankAccountCommand::Actions::deposit, 200},
        BankAccountCommand{ba, BankAccountCommand::Actions::deposit, 300},
        BankAccountCommand{ba, BankAccountCommand::Actions::withdraw, 1200},
        BankAccountCommand{ba, BankAccountCommand::Actions::withdraw, 200},
    };

    std::cout << ba;

    std::cout << "Calling all the comands\n";
    for(auto& a : bacv)
    {
        a.call();
    }

    std::cout << "And now undo them\n";
    for(auto it = bacv.rbegin(); it != bacv.rend(); it++)
    {
        it->undo();
    }

    // more compicated approach for dependant commands
    // testing on the same commands and bank account
    CompositeBankAccountCommand commands{
        BankAccountCommand{ba, BankAccountCommand::Actions::deposit, 200},
        BankAccountCommand{ba, BankAccountCommand::Actions::deposit, 300},
        BankAccountCommand{ba, BankAccountCommand::Actions::withdraw, 1200},
        BankAccountCommand{ba, BankAccountCommand::Actions::withdraw, 200},
    };

    std::cout << "Calling all the comands from vector. Start state: " << ba;

    commands.call();
    std::cout << "After calling: " << ba;

    commands.undo();
    std::cout << "After undoing: " << ba;

    std::cout << "Money transfer\n";
    BankAccount ba1, ba2;
    ba1.deposit(1000);
    std::cout << "Account 1   " << ba1;
    std::cout << "Account 2   " << ba2;

    MoneyTransferCommand mtc{ba1, ba2, 500};
    mtc.call();
    std::cout << "Account 1   " << ba1;
    std::cout << "Account 2   " << ba2;

    std::cout << "Undo transfer\n";
    mtc.undo();
    std::cout << "Account 1   " << ba1;
    std::cout << "Account 2   " << ba2;

     std::cout << "Transferring more than we can\n";
    MoneyTransferCommand mtc2{ba1, ba2, 1600};
    mtc2.call();
    std::cout << "Account 1   " << ba1;
    std::cout << "Account 2   " << ba2;

    std::cout << "Undo transfer\n";
    mtc2.undo();
    std::cout << "Account 1   " << ba1;
    std::cout << "Account 2   " << ba2;
}

