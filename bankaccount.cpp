#include <iostream>
#include <iomanip>

class Account 
{
    public:
         Account(const std::string &accNum, const std::string &name, double initialBal)
        : accountNumber(accNum), accountHolder(name), balance(initialBal) {} // default constructor
        void displayDetails()
        {
            std::cout << "Account Details for " << getAccType() << " Account (ID: " << accountNumber 
            << "):\nHolder: " << accountHolder << std::fixed << std::setprecision(2) 
            << "\nBalance: $" << balance << std::endl;
        }

        void deposit(double amount) 
        {
            // no reqs to deposit money so just add to balance
            balance += amount; 
        }

        void withdraw(double amount) 
        {
            if (amount > balance) // amount greater than funds
            {
                std::cout << "Insufficient funds for withdrawal." << std::endl;
            }
            else // passes requirement to withdraw
            {
                balance -= amount;
            }
        }

        friend void operator+(Account& acc1, Account& acc2);
        friend ostream& operator<<(ostream& os, const Account& acc);

    // we want to be able to access these members in the derived classes
    protected:
        std::string accountNumber;
        std::string accountHolder;
        double balance;
        std::string getAccType() const 
        {
            return "Base";
        }
};

class SavingsAccount : public Account
{
    public:
        SavingsAccount(const std::string accNum, const std::string name, double initialBal, double interest)
        : Account(accNum, name, initialBal), interestRate(interest) {} // default constructor

        void displayDetails()
        {
            std::cout << Account << "\nInterest Rate: " << double(interestRate * 100) << "%" << std::endl;
        }

        void withdraw(double amount)
        {
            if ((balance - amount) < minBalance) // minimum balance requirement not met
            {
                std::cout << "Withdrawal not allowed. Minimum balance requirement not met." << std::endl;
            }
            else if (amount > balance) // amount greater than funds
            {
                std::cout << "Insufficient funds for withdrawal." << std::endl;
            }
            else // passes all requirements to withdraw
            {
                balance -= amount;
            }
        }

    // savings account is the only account to use interestRate, so it is private
    private:
        double interestRate;
        const double minBalance = 100.00;
    
    // we want to override the base class getAccType so that we get the correct type printed
    protected:
        std::string getAccType() const
        {
            return "Savings";
        }
};

class CurrentAccount : public Account
{
    public:
        CurrentAccount(const std::string accNum, const std::string name, double initialBal, double overdraft)
        : Account(accNum, name, initialBal), overdraftlimit(overdraft) {} // default constructor

        void displayDetails()
        {
            std::cout << Account << "\nOverdraft Limit: $" << overdraftlimit <<std::endl;
        }

        void withdraw(double amount)
        {
            if ((balance - amount) <= (-overdraftlimit)) // exceeds overdraft limit
            {
                std::cout << "Withdrawal not allowed. Exceeds overdraft limit." << std::endl;
            }
            else if (amount > balance) // amount greater than funds
            {
                std::cout << "Insufficient funds for withdrawal" << std::endl;
            }
            else // passes all requirements to withdraw
            {
                balance -= amount;
            }
        }

    private:
        double overdraftlimit;

    protected:
        std::string getAccType() const
        {
            return "Current";
        }
};

void operator+(Account& acc1, Account& acc2)
{
    double amount = 300; 
    if (acc2.balance >= amount)
    {
        // we want to withdraw from acc2 to put into acc1
        acc2.withdraw(amount); 
        acc1.deposit(amount); 
    }
    else // if balance less than amount
    {
        std::cout << "Insufficient funds for transfer." << std::endl;
    }
}

ostream& operator<<(ostream& os, const Account& acc)
{
    os << "Account Details for " << acc.getAccType() << " Account (ID: " << acc.accountNumber 
    << "):\nHolder: " << acc.accountHolder << std::fixed << std::setprecision(2) 
    << "\nBalance: $" << acc.balance << std::endl;
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000.00, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000.00, 500.00);
    
    savings.displayDetails();
    std::cout << std::endl;
    current.displayDetails();
    std::cout << std::endl;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "Account Details after deposit and withdrawal:" << std::endl;
    savings.displayDetails();
    std::cout << std::endl;
    current.displayDetails();
    std::cout << std::endl;

    std::cout << "Account Details after transfer:" << std::endl;
    // transfer $300 from savings to current
    current + savings;
    savings.displayDetails();
    std::cout << std::endl;
    current.displayDetails();

    return 0;
}