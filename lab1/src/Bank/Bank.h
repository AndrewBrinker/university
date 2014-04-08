/**
 * Copyright 2014 Andrew Brinker
 */

#ifndef BANK_H
#define BANK_H

#include <Account/Account.h>
#include <string>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class Bank {
  public:
    Bank();
    Bank(double, double);
    void deposit(double, std::string);
    void withdraw(double, std::string);
    void transfer(double, std::string);
    void printBalances();

  private:
    Account savings;
    Account checking;

    DISALLOW_COPY_AND_ASSIGN(Bank);
};

#endif  // BANK_H
