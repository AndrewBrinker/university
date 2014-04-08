/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Bank.h"
#include <Account/Account.h>
#include <string>
#include <iostream>

#define SAVINGS_ID  "S"
#define CHECKING_ID "C"

Bank::Bank()
    : savings(), checking() {}

Bank::Bank(double savings_amount, double checking_amount)
    : savings(savings_amount), checking(checking_amount) {}

void Bank::deposit(double amount, std::string id) {
    if (id == SAVINGS_ID) {
        savings.deposit(amount);
    } else if (id == CHECKING_ID) {
        checking.deposit(amount);
    } else {
        std::cout << "Invalid account ID." << std::endl;
    }
}

void Bank::withdraw(double amount, std::string id) {
    if (id == SAVINGS_ID) {
        savings.withdraw(amount);
    } else if (id == CHECKING_ID) {
        checking.withdraw(amount);
    } else {
        std::cout << "Invalid account ID." << std::endl;
    }
}

void Bank::transfer(double amount, std::string id) {
    if (id == SAVINGS_ID) {
        double value = savings.withdraw(amount);
        checking.deposit(value);
    } else if (id == CHECKING_ID) {
        double value = checking.withdraw(amount);
        savings.deposit(value);
    } else {
        std::cout << "Invalid account ID." << std::endl;
    }
}

void Bank::printBalances() {
    std::cout << "Savings balance: " << savings.query() << std::endl;
    std::cout << "Checking balance: " << checking.query() << std::endl;
}
