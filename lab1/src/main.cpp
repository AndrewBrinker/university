/**
 * Copyright 2014 Andrew Brinker
 */

#include <Account/Account.h>
#include <Bank/Bank.h>
#include <iostream>

#define START_VALUE   10000
#define TARGET_VALUE  (START_VALUE * 2)
#define INTEREST_RATE 6

int main() {
    /**
     * First Account Tests
     */

    Account savings(100);
    savings.deposit(20);
    std::cout << savings.query() << std::endl;
    if (savings.withdraw(50)) {
        std::cout << "Successful withdrawal.\n";
    } else {
        std::cout << "Not enough in the account, $5 penalty charged.\n";
    }
    std::cout << savings.query() << std::endl;
    if (savings.withdraw(100)) {
        std::cout << "Successful withdraw.\n";
    } else {
        std::cout << "Not enough in the account, $5 penalty charged.\n";
    }
    std::cout << savings.query() << std::endl;

    /**
     * Second Account Tests
     */

    Account savings2(START_VALUE, INTEREST_RATE);
    int years = 0;
    while (savings2.query() < TARGET_VALUE) {
        savings2.addInterest();
        ++years;
    }
    std::cout << "It will take " << years << " years." << std::endl;

    /**
     * Bank Tests
     */

    Bank csusb_bank(1000, 100);
    csusb_bank.deposit(50, "S");
    csusb_bank.withdraw(10, "CH");
    csusb_bank.withdraw(10, "C");
    csusb_bank.transfer(40, "S");
    csusb_bank.withdraw(150, "C");
    csusb_bank.printBalances();

    return 0;
}
