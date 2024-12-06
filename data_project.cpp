#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <windows.h>
#include <locale>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <ios>
time_t currentTime = time(NULL); // to get the current time
struct tm *localTime = localtime(&currentTime);

using namespace std;
using namespace chrono;
struct transaction
{
    string date;
    double amount;
    transaction *next;
};
struct account
{
    string IBAN;
    string accountName;
    double balance;
    string currency;
    double limitDepositPerDay;
    double limitWithdrawPerMonth;
    transaction *txn;
    account *next;
};
struct user
{
    int userID;
    string fname;
    string lname;
    account *acct;
    user *next, *previous;
};
struct userList
{
    user *head, *tail;
};

void choice(userList *, int);
void access(userList *list, int size);
void accesschoice(userList *list, int size, int ID);
void print(userList *);
void access_transaction(userList *, int);
void transaction_access_2_accounts(userList *, int );

// the function read from file is done and tested every thing is working fine including the transactions
userList *readFromFile(userList *list)
{

    fstream input("input.txt", ios::in);
    string line;
    user *currentUser = NULL;
    account *currentAccount = NULL;

    while (getline(input, line))
    {

        if (line[0] == '-')
        {
            line = line.substr(1);
            stringstream ss(line);
            string userIDStr, fname, lname;

            getline(ss, userIDStr, ',');
            getline(ss, fname, ',');
            getline(ss, lname);

            user *newUser = new user;
            newUser->userID = stoi(userIDStr);
            newUser->fname = fname;
            newUser->lname = lname;
            newUser->acct = NULL;
            newUser->next = NULL;
            newUser->previous = NULL;

            if (list->head == NULL)
            {
                list->head = newUser;
                list->tail = newUser;
            }
            else
            {
                newUser->previous = list->tail;
                list->tail->next = newUser;
                list->tail = newUser;
            }
            currentUser = newUser;
        }

        else if (line[0] == '#')
        {
            line = line.substr(1);
            stringstream ss(line);
            string IBAN, accountName, balanceStr, limitDStr, limitWStr;

            getline(ss, IBAN, ',');
            getline(ss, accountName, ',');
            getline(ss, balanceStr, ',');
            getline(ss, limitDStr, ',');
            getline(ss, limitWStr);

            account *newAccount = new account;
            newAccount->IBAN = IBAN;
            newAccount->accountName = accountName;
            newAccount->txn = NULL;
            newAccount->next = NULL;

            string currency;
            double balance, limitD, limitW;

            int pos = balanceStr.find_first_not_of("0123456789.");

            balance = stold(balanceStr.substr(0, pos));
            currency = balanceStr.substr(pos);
            pos = limitDStr.find_first_not_of("0123456789.");
            limitD = stold(limitDStr.substr(0, pos));
            pos = limitWStr.find_first_not_of("0123456789.");
            limitW = stold(limitWStr.substr(0, pos));

            newAccount->balance = balance;
            newAccount->currency = currency;
            newAccount->limitDepositPerDay = limitD;
            newAccount->limitWithdrawPerMonth = limitW;

            if (currentUser->acct == NULL)
            {
                currentUser->acct = newAccount;
            }
            else
            {
                account *temp = currentUser->acct;
                while (temp->next)
                {
                    temp = temp->next;
                }
                temp->next = newAccount;
            }
            currentAccount = newAccount;
        }
        else if (line[0] == '*')
        {
            line = line.substr(1);
            stringstream ss(line);
            string date, amountStr;
            getline(ss, date, ',');
            getline(ss, amountStr);

            transaction *newTransaction = new transaction;
            newTransaction->date = date;

            int pos = amountStr.find_first_not_of("0123456789.-");

            newTransaction->amount = stold(amountStr.substr(0, pos));
            newTransaction->next = NULL;

            if (currentAccount->txn == NULL)
            {
                currentAccount->txn = newTransaction;
            }
            else
            {
                transaction *temp = currentAccount->txn;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newTransaction;
            }
        }
    }

    input.close();
    return list;
}

// prints every thing this may be deleted later on
void print(userList *list)
{
    user *travel = list->head;
    while (travel != NULL)
    {
        cout << "-" << travel->userID << setw(20) << travel->fname << setw(20) << travel->lname << endl;

        account *currentAccount = travel->acct;
        while (currentAccount != NULL)
        {
            cout << "#" << currentAccount->IBAN << "," << currentAccount->accountName << ","
                 << currentAccount->balance << currentAccount->currency << ","
                 << currentAccount->limitDepositPerDay << ","
                 << currentAccount->limitWithdrawPerMonth << endl;

            transaction *currentTxn = currentAccount->txn;
            while (currentTxn != NULL)
            {
                cout << "*" << currentTxn->date << "," << currentTxn->amount << endl;
                currentTxn = currentTxn->next;
            }

            currentAccount = currentAccount->next;
        }
        travel = travel->next;
    }
    return;
}
int sizereaderusr()
{
    fstream file;
    int size = 0;
    string line;
    file.open("input.txt", ios::in);
    while (getline(file, line))
    {
        if (line[0] == '-')
        {

            size++;
        }
    }
    file.close();
    return size;
}
void checkname(string &name)
{
    bool x;
    do
    {
        x = true;
        getline(cin, name);
        for (char c : name)
        {
            if (!isalpha(c))
            {
                x = false;
                break;
            }
        }
        if (name.length() < 1)
            x = false;
        if (!x)
            cout << "\033[1;31mInvalid text.\033[0m\nPlease enter a name:";

    } while (!x);
}

bool repetion(user *head, string first, string last)
{
    user *travel = head;
    while (travel != NULL)
    {
        if (travel->fname == first && travel->lname == last)
        {
            cout << "\033[1;31m--> There is an account with this name.\033[0m\n";
            return true;
        }
        travel = travel->next;
    }
    return false;
}

string allocation_input()
{
    string choice;
    bool x;
    do
    {
        cout << "To access press '1' to sighn-up press '2': ";
        getline(cin, choice);
        cout << choice.length();
        if ((choice != "1" || choice != "2") && choice.length() != 1)
        {
            cout << "\033[1;31mInvalid input.\033[0m\n";
            x = false;
        }
        else
            x = true;
    } while (x == false);
    return choice;
}

void insert(userList *&list, user *newUser)
{
    if (list->head == NULL)
    {
        list->head = newUser;
        list->tail = newUser;
    }
    else
    {
        list->tail->next = newUser;
        list->tail = newUser;
    }
    return;
}
void AddNewUser(userList *&list, int &val)
{
    string first, last;
    bool x;
    val += 1;
    string q = "2";
    do
    {
        cout << "--> Enter First Name:  ";
        checkname(first);
        cout << "-->Enter Last Name:  ";
        checkname(last);
        x = repetion(list->head, first, last);
        if (x == true)
        {
            q = allocation_input();
            if (q == "1")
            {
                access(list, val);
                break;
            }
        }
    } while (x == true);

    if (q == "2")
    {
        user *newUser = new user;
        newUser->userID = val;
        newUser->fname = first;
        newUser->lname = last;
        newUser->acct = NULL;
        newUser->next = NULL;
        newUser->previous = NULL;
        insert(list, newUser);
        cout << "\033[1;32m--> An account has been created....\nYour ID is: " << val << "\033[0m\n";
        choice(list, val);
    }
    return;
}
user *checklogin(user *head, string first, string last, int ID)
{
    user *travel = head;
    while (travel != NULL)
    {
        if (travel->fname == first && travel->lname == last && travel->userID == ID)
        {
            return travel;
        }
        // travel->previous = travel;
        travel = travel->next;
    }
    cout << "\033[1;31m--> There is no account with this information.\033[0m\n";
    return NULL;
}
void account_type(string &type)
{
    bool x;
    string s;
    do
    {
        x = true;
        cout << "\033[1;33m-->Press 1 if the account is 'international'\n-->Press 2 if the account is 'domiciled'\033[0m\n";
        getline(cin, s);
        if (s.length() != 1)
        {
            cout << "\033[1;31m--> Invalid choice.\033[0m\n";
            x = false;
        }
        if (s == "1")
        {
            type = "international";
        }
        else if (s == "2")
        {
            type = "domiciled";
        }
        else
            x = false;
    } while (x == false);
    return;
}
void currency_get(string &currency)
{
    do
    {
        cout << "\033[1;33mEnter the currency of the new account( $/ L.L/press 1 for \u20AC): \033[0m";
        getline(cin, currency);
        if (currency != "L.L" && currency != "$" && currency != "1")
        {
            cout << "\033[1;31m--> Invalid choice.\033[0m\n";
        }
        // € is not working?
    } while (currency != "L.L" && currency != "$" && currency != "1");
    if (currency == "1")
        currency = "\u20AC";
}
void Deposit_check(double &balance) {
    string blc;
    bool x;
    int z, y;
    do {
        z = y = 0;
        getline(cin, blc);
        for (int i = 0; i < blc.length(); i++) {
            if (blc[i] == '.') {
                z++;
            }
            if (!isdigit(blc[i]) && blc[i] != '.') {
                y++;
            }
        }
        for (char c : blc) {
            if (!isdigit(c)) {
                y++;
                break;
            }
            if ((z == 1 || z == 0) && y == 0) {
                x = true;
            } else {
                x = false;
            }
        }
        if (x == false) {
            cout << "\033[1;31m--> Invalid choice.\033[0m\n\033[1;32m-->Enter the correct form:  \033[0m";
        }
    } while (x == false);
    balance = stod(blc);
    return;
}
void IBAN_confirm(user *tmp, string &IBAN)
{
    bool isUnique, x;
    do
    {
        isUnique = true;
        cout << "\033[1;33m--> Enter the IBAN of the new account: \033[0m";
        getline(cin, IBAN);
        user *tmps = tmp;
        while (tmps != NULL)
        {
            account *tmpacc = tmps->acct;
            while (tmpacc != NULL)
            {
                if (IBAN == tmpacc->IBAN)
                {
                    cout << "\033[1;31m--> This IBAN already exists, please enter a different one.\n \033[0m";
                    isUnique = false;
                    break;
                }
                tmpacc = tmpacc->next;
            }
            if (isUnique == false)
            {
                break;
            }
            tmps = tmps->next;
        }
    } while (!isUnique);
    return;
}

void add_account_to_user(userList *&list, user *targetUser, account *newAccount)
{
    if (targetUser->acct == NULL)
    {
        targetUser->acct = newAccount;
    }
    else
    {
        account *current = targetUser->acct;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newAccount;
    }
    return;
}

void add_new_Account(userList *list, int size, int ID)
{
    user *tmp = list->head;
    while (tmp != NULL && tmp->userID != ID)
    {
        tmp = tmp->next;
    }

    string IBAN, type, currency;
    double balance, limitDPD, limitWPM;
    IBAN_confirm(tmp, IBAN);
    account_type(type);
    currency_get(currency);
    cout << "\033[1;33mEnter the Balance of the new account: \033[0m";
    Deposit_check(balance);
    cout << "\033[1;33mEnter the limit Deposit PerDay of the new account: \033[0m";
    Deposit_check(limitDPD);
    cout << "\033[1;33mEnter the limit Withdraw PerMonth of the new account: \033[0m";
    Deposit_check(limitWPM);

    account *newAccount = new account;
    newAccount->IBAN = IBAN;
    newAccount->accountName = type;
    newAccount->balance = balance;
    newAccount->currency = currency;
    newAccount->limitDepositPerDay = limitDPD;
    newAccount->limitWithdrawPerMonth = limitWPM;
    newAccount->txn = NULL;
    newAccount->next = NULL;

    add_account_to_user(list, tmp, newAccount);
    choice(list, size);
    return;
}

void ID_checker(int &ID)
{
    string id;
    bool x;
    do
    {
        cout << "\033[1;33m-->Enter your ID: \033[0m";
        getline(cin, id);
        for (char c : id)
        {
            if (!isdigit(c))
            {
                x = false;
                break;
            }
            else
                x = true;
        }
    } while (x == false);
    ID = stoi(id);
}
void access(userList *list, int size)
{
    int ID;
    user *chosen;
    do
    {
        string firstname, lastname;
        ID_checker(ID);
        cout << "\033[1;33m--> Enter your first name: \033[0m";
        checkname(firstname);
        cout << "\033[1;33m--> Enter your last name: \033[0m";
        checkname(lastname);
        chosen = checklogin(list->head, firstname, lastname, ID);
    } while (chosen == NULL);
    add_new_Account(list, size, ID);
    return;
}

// this function is done but not tested for the transaction function
void add_to_txt_file(userList *list)
{
    fstream input;
    user *travel = list->head;
    int q = 0;
    input.open("input.txt", ios::out);

    while (travel != NULL)
    {
        if (q == 0)
        {
            input << "-" << travel->userID << "," << travel->fname << "," << travel->lname;
            q++;
        }
        else
        {
            input << endl
                  << "-" << travel->userID << "," << travel->fname << "," << travel->lname;
        }

        account *tmp2 = travel->acct;
        while (tmp2 != NULL)
        {
            input << endl
                  << "#" << tmp2->IBAN << "," << tmp2->accountName << ","
                  << tmp2->balance << tmp2->currency << ","
                  << tmp2->limitDepositPerDay << tmp2->currency << ","
                  << tmp2->limitWithdrawPerMonth << tmp2->currency;

            transaction *tmp3 = tmp2->txn;
            while (tmp3 != NULL)
            {
                input << endl
                      << "*" << tmp3->date << "," << tmp3->amount << tmp2->currency;
                tmp3 = tmp3->next;
            }
            tmp2 = tmp2->next;
        }
        travel = travel->next;
    }
    input.close();
    return;
}

double convert_currency(string currency_1, string currency_2,double amount) {
    double LL_rate = 89000;  
    double euro_rate = 0.85;   
    
    if (currency_1 == currency_2) {
        return amount;
    }
    
    double usd_amount;
    
    if (currency_1 == "L.L") {
        usd_amount = amount / LL_rate;
    } else if (currency_1 != "$" && currency_1 != "L.L") {
        usd_amount = amount / euro_rate;
    } else { 
        usd_amount = amount;
    }
    
    if (currency_2 == "L.L") {
        return usd_amount * LL_rate;
    } else if (currency_2 != "$" && currency_2 != "L.L") {
        return usd_amount * euro_rate;
    } else {  
        return usd_amount;
    }
}

account* findAccountByIBAN(userList* list, string& IBAN) {
    account* foundAccount = NULL;
    do {
        getline(cin, IBAN);
        user* currentUser = list->head;
        while (currentUser != NULL) {
            account* currentAccount = currentUser->acct;
            while (currentAccount != NULL) {
                if (currentAccount->IBAN == IBAN) {
                    foundAccount = currentAccount;
                    return foundAccount;
                }
                currentAccount = currentAccount->next;
            }
            currentUser = currentUser->next;
        }
        
        if (foundAccount==NULL) {
            cout << "\033[1;31mNo account found with this IBAN.\033[0m" << endl;
            cout << "\033[1;33mPlease re-enter the IBAN: \033[0m";
        }
    } while (foundAccount==NULL);
    
    return foundAccount;
}
void transfer(userList* &list, account* &account1, account* &account2, double ammount,int size){
    ostringstream dnow;
    dnow << std::put_time(localTime, "%d/%m/%Y");
    string dateString = dnow.str();
    bool valid_amount = false;
    double converted_ammount;
    while (valid_amount==false) {
        if (ammount > account1->limitWithdrawPerMonth) {
            cout << "\033[1;31m--> The ammount is greater than the withdraw limit of the account.\033[0m\n";
            cout << "\033[1;33mPlease re-enter the amount: \033[0m";
            Deposit_check(ammount);
            continue;
        }
        
        converted_ammount = convert_currency(account1->currency, account2->currency, ammount);
        if (converted_ammount > account2->limitDepositPerDay) {
            cout << "\033[1;31m--> The ammount is greater than the deposit limit of the receiving account.\033[0m\n";
            cout << "\033[1;33mPlease re-enter the amount: \033[0m";
            Deposit_check(ammount);
            continue;
        }
        
        valid_amount = true;
    }
    account1->balance -= ammount;
    cout<<"\033[1;33m--> Enter the new limit of the withdraw per month(of the first account): \033[0m";
    Deposit_check(account1->limitWithdrawPerMonth);
    cout<<"\033[1;33m--> Enter the new limit of the deposit per day(of the first account): \033[0m";
    Deposit_check(account1->limitDepositPerDay);
    cout<<"\033[1;33m--> Enter the new limit of the withdraw per month(of the second account): \033[0m";
    Deposit_check(account2->limitWithdrawPerMonth);
    cout<<"\033[1;33m--> Enter the new limit of the deposit per day(of the second account): \033[0m";
    Deposit_check(account2->limitDepositPerDay);

    transaction *tmp = account1->txn;
    if(tmp==NULL){
        account1->txn = new transaction;
        account1->txn->date = dateString;
        account1->txn->amount = -ammount;
        account1->txn->next = NULL;
    }
    else{
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next = new transaction;
    tmp->next->date = dateString;
    tmp->next->amount = -ammount;
    tmp->next->next = NULL;
    account2->balance += converted_ammount;
        
    }
    tmp = account2->txn;
    if(tmp==NULL){
        account2->txn = new transaction;
        account2->txn->date = dateString;
        account2->txn->amount = converted_ammount;
        account2->txn->next = NULL;
    }
    else{
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next = new transaction;
    tmp->next->date = dateString;
    tmp->next->amount = converted_ammount;
    tmp->next->next = NULL;
    }
    cout << "\033[1;32m--> The ammount has been transferred successfully.\033[0m\n";
    return;
}


void transaction_access_2_accounts(userList *list, int size)
{
    string IBAN1, IBAN2;
    double ammount;
    cout << "\033[1;33mEnter the IBNA of the first User\n\033[0m";
    account* account1 = findAccountByIBAN(list, IBAN1);
    cout << "\033[1;33mEnter the IBNA of the second User\n \033[0m";
    account* account2 = findAccountByIBAN(list, IBAN2);
    cout << "\033[1;33mEnter the ammount that you want to transfer(plase note that the currency will be the same as the first account): \033[0m";
    Deposit_check(ammount);

    transfer(list, account1, account2, ammount,size);
    choice(list, size);
}

//assaf ???
void sdatecheck(string &date)
{
   bool x;
    auto now = chrono::system_clock::now();
    auto yesterday = now - chrono::hours(24);
    do {
        cout << "-->";
        x = true;
        getline(cin, date);
        tm time = {};                         // in ctime lib
        istringstream datee(date);            // s stream
        datee >> get_time(&time, "%d/%m/%Y"); // function format
        if (datee.fail()) {
            cout << "\033[1;31mInvalid format, please enter a valid one.\033[0m\n";
            x = false;
        } else {
            auto usertime = chrono::system_clock::from_time_t(mktime(&time)); // user input time
            
            if (usertime > yesterday) {
                cout << "\033[1;31mPlease enter a date from the past.\033[0m\n";
                x = false;
            }
        }
    } while (!x );

return;
}

userList *delete_transactions(userList *list, string date) {
    tm inputTime = {};
    istringstream dateStream(date);
    dateStream >> get_time(&inputTime, "%d/%m/%Y");
    
    user *tmp = list->head;
    while(tmp != NULL) {
        account *tmpacc = tmp->acct;
        while(tmpacc != NULL) {
            transaction *tmptran = tmpacc->txn;
            transaction *prev = NULL;
            
            while(tmptran != NULL) {
                tm transactionTime = {};
                istringstream transStream(tmptran->date);
                transStream >> get_time(&transactionTime, "%d/%m/%Y");
                
                auto inputDateTime = system_clock::from_time_t(mktime(&inputTime));
                auto transactionDateTime = system_clock::from_time_t(mktime(&transactionTime));
                
                if(inputDateTime >= transactionDateTime) {
                    if(prev == NULL) {
                        tmpacc->txn = tmptran->next;
                        delete tmptran;
                        tmptran = tmpacc->txn;
                    } else {
                        prev->next = tmptran->next;
                        delete tmptran;
                        tmptran = prev->next;
                    }
                } else {
                    prev = tmptran;
                    tmptran = tmptran->next;
                }
            }
            tmpacc = tmpacc->next;
        }
        tmp = tmp->next;
    }
    return list;
}

void delete_devider(userList * list,int size){
    string date;
    sdatecheck(date);
    list=delete_transactions(list, date);
    access_transaction(list,size);
}

void access_transaction(userList *list, int size)
{
    char x;
    cout << endl
         << "\033[1;33m--> ~Press 1 to start a transaction btween 2 users .\n--> ~Press 2 to delete transactions before a certain date .\n--> ~Press 0 to Return.\033[0m " << endl;
    do
    {
        cin >> x;
        if (x == '1')
        {
            cin.ignore();
            transaction_access_2_accounts(list, size);
        }
        else if (x == '2')
        {
            cin.ignore();
            delete_devider(list,size);
        }
        else if (x == '0')
        {
            choice(list, size);
            return;
        }
        else
        {
            cout << "\033[1;31m--> Invalid choice.\033[0m\n";
        }
    } while (x != '1' && x != '2' && x != '0');
    return;
}

bool comparedate(string date, string d)
{
    tm time = {}, time2 = {};
    istringstream datee1(date);
    datee1 >> get_time(&time, "%d/%m/%Y");
    istringstream datee2(d);
    datee2 >> get_time(&time2, "%d/%m/%Y");

    auto t1 = chrono::system_clock::from_time_t(mktime(&time));
    auto t2 = chrono::system_clock::from_time_t(mktime(&time2));
    if (t1 > t2)
    {
        return false;
    }

    return true;
}

userList* sorting(userList* list) {
    user* tmp = list->head;
    while (tmp != NULL) {
        account* tmp2 = tmp->acct;
        while (tmp2 != NULL) {
            bool swapped;
            transaction* ptr1;
            transaction* lptr = NULL;

            // If list is empty or has only one element
            if (tmp2->txn == NULL || tmp2->txn->next == NULL) {
                tmp2 = tmp2->next;
                continue;
            }

            do {
                swapped = false;
                ptr1 = tmp2->txn;

                while (ptr1->next != lptr) {
                    if (comparedate(ptr1->next->date, ptr1->date)) {
                        // Swap nodes
                        transaction* ptr2 = ptr1->next;
                        
                        // Update head if necessary
                        if (ptr1 == tmp2->txn) {
                            tmp2->txn = ptr2;
                        } else {
                            transaction* prev = tmp2->txn;
                            while (prev->next != ptr1) {
                                prev = prev->next;
                            }
                            prev->next = ptr2;
                        }

                        ptr1->next = ptr2->next;
                        ptr2->next = ptr1;
                        // Update ptr1 for next iteration
                        ptr1 = ptr2;
                        
                        swapped = true;
                    }
                    ptr1 = ptr1->next;
                }
                lptr = ptr1;
            } while (swapped);

            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
    return list;
}

void choice(userList *userlist, int size)
{
    char x;
    cout << "\033[1;33m--> ~Press 1 to access a account(to add a new account).\n--> ~Press 2 to add a account.\n--> ~Press 3 to access the transaction menu.\n--> ~Press 0 to exit.\033[0m " << endl;
    do
    {
        cin >> x;
        if (x == '2')
        {
            cin.ignore();
            AddNewUser(userlist, size);
        }
        else if (x == '1')
        {
            cin.ignore();
            access(userlist, size);
        }
        else if (x == '3')
        {
            cin.ignore();
            access_transaction(userlist, size);
        }
        else if (x == '0')
        {
            userlist=sorting(userlist);
            add_to_txt_file(userlist);
            cout << "\033[30m-->Thanks for using our service.<--\033[0m\n";
        }
        else
        {
            cout << "\033[1;31m--> Invalid choice.\033[0m\n";
        }
    } while (x != '1' && x != '2' && x != '3' && x != '0');
    return;
}
int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8); // Set console to UTF-8
    userList *list = new userList;
    list->head = NULL;
    list->tail = NULL;
    list = readFromFile(list);
    int size = sizereaderusr();
    // transaction_btw_2accs(list, size);
    cout << endl;
    choice(list, size);
    // the print function is changing the format after it prints so it the lists wont work after entering it be care full change it later also fix the setw() in it so it can calculat the length
    print(list);
    delete list;
}