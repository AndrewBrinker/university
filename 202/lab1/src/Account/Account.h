/**
 * Copyright 2014 Andrew Brinker
 */

#ifndef ACCOUNT_H
#define ACCOUNT_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class Account {
  public:
    Account();
    Account(double);
    Account(double, double);
    void deposit(double);
    bool withdraw(double);
    double query();

    void setInterestRate(double);
    double getInterestRate();
    void addInterest();

  private:
    double balance;
    double interest_rate;

    DISALLOW_COPY_AND_ASSIGN(Account);
};

#endif  // ACCOUNT_H
