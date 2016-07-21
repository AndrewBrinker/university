/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Account.h"

#define PENALTY 5

Account::Account()
    : balance(0), interest_rate(0) {}

Account::Account(double amount)
    : balance(amount), interest_rate(0) {}

Account::Account(double amount, double rate)
    : balance(amount), interest_rate(rate) {}

void Account::deposit(double amount) {
    balance += amount;
}

bool Account::withdraw(double amount) {
    if (amount > balance) {
        balance -= PENALTY;
        return false;
    }
    balance -= amount;
    return true;
}

double Account::query() {
    return balance;
}

void Account::setInterestRate(double rate) {
    interest_rate = rate;
}

double Account::getInterestRate() {
    return interest_rate;
}

void Account::addInterest() {
    balance += (balance * interest_rate) / 100;
}
