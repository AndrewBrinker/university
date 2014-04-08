/**
 * Copyright 2014 Andrew Brinker
 */

#include <Account/Account.h>
#include <Bank/Bank.h>
#include <cstdio>

#define START_VALUE   10000
#define TARGET_VALUE  (START_VALUE * 2)
#define INTEREST_RATE 6

int main() {
    /**
     * First Account Tests
     */

    Account savings(100);
    savings.deposit(20);

    printf("Current funds: %f\n", savings.query());

    if (savings.withdraw(50)) {
        printf("Successful withdrawal\n");
    } else {
        printf("Insufficient funds. $5 penalty charged\n");
    }

    printf("Current funds: %f\n", savings.query());

    if (savings.withdraw(100)) {
        printf("Successful withdrawal\n");
    } else {
        printf("Insufficient funds. $5 penalty charged\n");
    }

    printf("Current funds: %f\n", savings.query());

    /**
     * Second Account Tests
     */

    Account savings_with_interest(START_VALUE, INTEREST_RATE);
    int years = 0;

    while (savings_with_interest.query() < TARGET_VALUE) {
        savings_with_interest.addInterest();
        ++years;
    }

    printf("It will take %d years\n", years);

    /**
     * Bank Tests
     */

    Bank bank(1000, 100);
    bank.deposit(50, "S");
    bank.withdraw(10, "CH");
    bank.withdraw(10, "C");
    bank.transfer(40, "S");
    bank.withdraw(150, "C");
    bank.printBalances();

    return 0;
}
