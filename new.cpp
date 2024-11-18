#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Define structures for users, accounts, and transactions
struct transaction {
    std::string date;
    double amount;
    transaction *next;
};

struct account {
    std::string IBAN;
    std::string accountName;
    double balance;
    std::string currency;
    double limitDepositPerDay;
    double limitWithdrawPerMonth;
    transaction *txn;
    account *next;
};

struct user {
    int userID;
    std::string fname;
    std::string lname;
    account *acct;
    user *next, *previous;
};

struct userList {
    user *head, *tail;
};

// Function to compare dates (implement your date comparison logic here)
int compareDates(const std::string& date1, const std::string& date2) {
    // Assume simple lexicographical comparison for now (YYYY/MM/DD format)
    return date1.compare(date2);
}

// Function to get the current date (placeholder, replace with actual date function)
std::string getCurrentDate() {
    return "15/11/2024";
}

// Function to parse the input file and build the userList
userList* parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return nullptr;
    }

    userList* users = new userList();
    users->head = users->tail = nullptr;

    std::string line;
    user* currentUser = nullptr;
    account* currentAccount = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line[0] == '-') {
            // New user
            std::istringstream ss(line.substr(1));
            currentUser = new user();
            ss >> currentUser->userID >> currentUser->fname >> currentUser->lname;
            currentUser->acct = nullptr;
            currentUser->next = currentUser->previous = nullptr;

            // Add user to userList (doubly linked list)
            if (users->head == nullptr) {
                users->head = users->tail = currentUser;
            } else {
                users->tail->next = currentUser;
                currentUser->previous = users->tail;
                users->tail = currentUser;
            }
        } else if (line[0] == '#') {
            // New account
            std::istringstream ss(line.substr(1));
            currentAccount = new account();
            ss >> currentAccount->IBAN >> currentAccount->accountName 
               >> currentAccount->balance >> currentAccount->limitDepositPerDay 
               >> currentAccount->limitWithdrawPerMonth;
            currentAccount->txn = nullptr;
            currentAccount->next = nullptr;

            // Add account to user's account list
            if (currentUser->acct == nullptr) {
                currentUser->acct = currentAccount;
            } else {
                account* temp = currentUser->acct;
                while (temp->next != nullptr) temp = temp->next;
                temp->next = currentAccount;
            }
        } else if (line[0] == '*') {
            // New transaction
            std::istringstream ss(line.substr(1));
            transaction* txn = new transaction();
            ss >> txn->date >> txn->amount;
            txn->next = nullptr;

            // Add transaction to account's transaction list
            if (currentAccount->txn == nullptr) {
                currentAccount->txn = txn;
            } else {
                transaction* temp = currentAccount->txn;
                while (temp->next != nullptr) temp = temp->next;
                temp->next = txn;
            }
        }
    }

    file.close();
    return users;
}

// Currency conversion function (placeholder)
double convertCurrency(const std::string& fromCurrency, const std::string& toCurrency, double amount) {
    // Add actual currency conversion logic here
    return amount;  // For now, return the amount unchanged
}

// Function to transfer an amount between two accounts
void transferAmount(userList* users, account* fromAcct, account* toAcct, double amount) {
    if (fromAcct->balance < amount) {
        std::cerr << "Insufficient funds!" << std::endl;
        return;
    }

    // Assuming a currency conversion function `convertCurrency`
    double convertedAmount = amount;
    if (fromAcct->currency != toAcct->currency) {
        convertedAmount = convertCurrency(fromAcct->currency, toAcct->currency, amount);
    }

    // Check limits
    if (amount > fromAcct->limitWithdrawPerMonth || convertedAmount > toAcct->limitDepositPerDay) {
        std::cerr << "Transaction exceeds limits!" << std::endl;
        return;
    }

    // Update balances
    fromAcct->balance -= amount;
    toAcct->balance += convertedAmount;

    // Create transactions
    transaction* withdrawTxn = new transaction();
    withdrawTxn->date = getCurrentDate(); // You need to implement this
    withdrawTxn->amount = -amount;
    withdrawTxn->next = fromAcct->txn;
    fromAcct->txn = withdrawTxn;

    transaction* depositTxn = new transaction();
    depositTxn->date = getCurrentDate(); // You need to implement this
    depositTxn->amount = convertedAmount;
    depositTxn->next = toAcct->txn;
    toAcct->txn = depositTxn;
}

// Function to sort transactions based on date (bubble sort implementation)
void sortTransactions(transaction*& txnList) {
    if (!txnList || !txnList->next) return;

    bool swapped;
    do {
        swapped = false;
        transaction* current = txnList;
        transaction* prev = nullptr;

        while (current->next) {
            if (compareDates(current->date, current->next->date) > 0) {
                // Swap the transactions
                if (prev) {
                    prev->next = current->next;
                } else {
                    txnList = current->next;
                }
                transaction* temp = current->next->next;
                current->next->next = current;
                current->next = temp;

                swapped = true;
            }
            prev = current;
            current = current->next;
        }
    } while (swapped);
}

// Function to delete old transactions before a given date
void deleteOldTransactions(transaction*& txnList, const std::string& cutoffDate) {
    while (txnList && compareDates(txnList->date, cutoffDate) < 0) {
        transaction* temp = txnList;
        txnList = txnList->next;
        delete temp;
    }

    transaction* current = txnList;
    while (current && current->next) {
        if (compareDates(current->next->date, cutoffDate) < 0) {
            transaction* temp = current->next;
            current->next = temp->next;
            delete temp;
        } else {
            current = current->next;
        }
    }
}

// Function to save the user list back to a file
void saveToFile(userList* users, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for writing!" << std::endl;
        return;
    }

    user* currentUser = users->head;
    while (currentUser) {
        file << "-" << currentUser->userID << "," << currentUser->fname << "," << currentUser->lname << "\n";
        account* currentAccount = currentUser->acct;
        while (currentAccount) {
            file << "#" << currentAccount->IBAN << "," << currentAccount->accountName << "," 
                 << currentAccount->balance << "," << currentAccount->limitDepositPerDay << "," 
                 << currentAccount->limitWithdrawPerMonth << "\n";

            transaction* currentTxn = currentAccount->txn;
            while (currentTxn) {
                file << "*" << currentTxn->date << "," << currentTxn->amount << "\n";
                currentTxn = currentTxn->next;
            }
            currentAccount = currentAccount->next;
        }
        currentUser = currentUser->next;
    }

    file.close();
}

// Main function for interacting with the banking system
int main() {
    userList* users = parseFile("input.txt");
    if (!users) return 1;

    // Main menu loop
    int choice;
    do {
        std::cout << "1. Transfer funds\n";
        std::cout << "2. Sort transactions\n";
        std::cout << "3. Delete old transactions\n";
        std::cout << "4. Save data\n";
        std::cout << "5. Exit\n";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                // Transfer funds logic
                // Example: transferAmount(users, fromAcct, toAcct, amount);
                break;
            }
            case 2: {
                // Sorting logic
                // Example: sortTransactions(account->txn);
                break;
            }
            case 3: {
                // Delete old transactions
                std::string cutoffDate;
                std::cout << "Enter cutoff date (DD/MM/YYYY): ";
                std::cin >> cutoffDate;
                user* currentUser = users->head;
                while (currentUser) {
                    account* currentAccount = currentUser->acct;
                    while (currentAccount) {
                        deleteOldTransactions(currentAccount->txn, cutoffDate);
                        currentAccount = currentAccount->next;
                    }
                    currentUser = currentUser->next;
                }
                break;
            }
            case 4: {
                saveToFile(users, "output.txt");
                break;
            }
        }
    } while (choice != 5);

    return 0;
}