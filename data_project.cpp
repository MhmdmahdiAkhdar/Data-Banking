#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cctype>
#include<cmath>
#include <windows.h>
#include <locale>
#include <iomanip>
// #include <ctime>
// #include <chrono>
// #include<ios>

// to get the current time
// time_t currentTime = time(NULL);
// struct tm *localTime = localtime(&currentTime);

using namespace std;

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
void print(userList*);


userList* readFromFile(userList* list) {
    
    fstream input("input.txt",ios::in);
    string line;
    user* currentUser = NULL;
    account* currentAccount = NULL;

    while (getline(input, line)) {

        if (line[0] == '-') {
            line = line.substr(1);
            stringstream ss(line);
            string userIDStr, fname, lname;
            
            getline(ss, userIDStr, ',');
            getline(ss, fname, ',');
            getline(ss, lname);

            user* newUser = new user;
            newUser->userID = stoi(userIDStr);
            newUser->fname = fname;
            newUser->lname = lname;
            newUser->acct = NULL;
            newUser->next = NULL;
            newUser->previous = NULL;

            if (list->head==NULL) {
                list->head = newUser;
                list->tail = newUser;
            } else {
                newUser->previous = list->tail;
                list->tail->next = newUser;
                list->tail = newUser;
            }
            currentUser = newUser;
        }
        
        else if (line[0] == '#') {
            line = line.substr(1);
            stringstream ss(line);
            string IBAN, accountName, balanceStr, limitDStr, limitWStr;
            
            getline(ss, IBAN, ',');
            getline(ss, accountName, ',');
            getline(ss, balanceStr, ',');
            getline(ss, limitDStr, ',');
            getline(ss, limitWStr);

            account* newAccount = new account;
            newAccount->IBAN = IBAN;
            newAccount->accountName = accountName;
            newAccount->txn = NULL;
            newAccount->next = NULL;

            string currency;
            double balance, limitD, limitW;
            
            int pos = balanceStr.find_first_not_of("0123456789.");
            
            balance = stod(balanceStr.substr(0, pos));
            currency = balanceStr.substr(pos);
            pos = limitDStr.find_first_not_of("0123456789.");
            limitD = stod(limitDStr.substr(0, pos));
            pos = limitWStr.find_first_not_of("0123456789.");
            limitW = stod(limitWStr.substr(0, pos));
            
            newAccount->balance = balance;
            newAccount->currency = currency;
            newAccount->limitDepositPerDay = limitD;
            newAccount->limitWithdrawPerMonth = limitW;

            if (currentUser->acct==NULL) {
                currentUser->acct = newAccount;
            }
            else {
                account* temp = currentUser->acct;
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = newAccount;
            }
            currentAccount = newAccount;
        }
        else if (line[0] == '*') {
            line = line.substr(1);
            stringstream ss(line);
            string date, amountStr;
            
            getline(ss, date, ',');
            getline(ss, amountStr);

            transaction* newTransaction = new transaction;
            newTransaction->date = date;
            
            int pos = amountStr.find_first_not_of("0123456789.");
            
            newTransaction->amount = stod(amountStr.substr(0, pos));
        
            newTransaction->next = currentAccount->txn;
            currentAccount->txn = newTransaction;
        }
    }

    input.close();
    return list;
}

// prints every thing this may be deleted later on
void print(userList* list) {
    user* travel = list->head;
    while (travel != NULL) {
        cout << "-" << travel->userID << setw(20) << travel->fname << setw(20) << travel->lname << endl;
        
        account* currentAccount = travel->acct;
        while (currentAccount != NULL) {
            cout << "#" << currentAccount->IBAN << "," << currentAccount->accountName << "," 
                 << currentAccount->balance << currentAccount->currency << "," 
                 << currentAccount->limitDepositPerDay << "," 
                 << currentAccount->limitWithdrawPerMonth << endl;
            
            transaction* currentTxn = currentAccount->txn;
            while (currentTxn != NULL) {
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

void insert(userList *&list, user *newUser) {
    if (list->head == NULL) {
        list->head = newUser;
        list->tail = newUser;
    } else {
        list->tail->next = newUser;
        list->tail = newUser;
    }
    return;
}
void AddNewUser(userList *&list, int &val) {
    string first, last;
    bool x;
    val += 1;
    string q = "2";
    do {
        cout << "--> Enter First Name:  ";
        checkname(first);
        cout << "-->Enter Last Name:  ";
        checkname(last);
        x = repetion(list->head, first, last); 
        if (x == true) {
            q = allocation_input();
            if (q == "1") {
                access(list, val);  
                break;
            }
        }
    } while (x == true);
    
    if (q == "2") {
        user* newUser = new user;
        newUser->userID = val;
        newUser->fname = first;
        newUser->lname = last;
        newUser->acct = NULL;
        newUser->next = NULL;
        newUser->previous = NULL;
        insert(list,newUser);
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
        travel->previous = travel;
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
void Deposit_check(double &balance)
{
    string blc;
    bool x;
    int z, y;
    do
    {
        z = y = 0;
        getline(cin, blc);
        for (int i = 0; i < blc.length(); i++)
        {
            if (blc[i] == '.')
            {
                z++;
            }
            if (!isdigit(blc[i]) && blc[i] != '.')
            {
                y++;
            }
        }
        for (char c : blc)
        {
            if (!isdigit(c))
            {
                y++;
                break;
            }
            if ((z == 1 || z == 0) && y == 0)
            {
                x = true;
            }
            else
                x = false;
        }
        if (x == false)
        {
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
        account *tmpacc = tmp->acct;
        while (tmpacc != NULL)
        {
            if (IBAN == tmpacc->IBAN)
            {
                cout << "\033[1;31m--> This IBAN already exists, please enter a different one:\n \033[0m";
                isUnique = false;
                break;
            }
            tmpacc = tmpacc->next;
        }
    } while (!isUnique );
    return;
}

void add_account_to_user(userList *&list, user *targetUser, account *newAccount) 
{
    if (targetUser->acct == NULL) {
        targetUser->acct = newAccount;
    } 
    else {
        account* current = targetUser->acct;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newAccount;
    }
    return;
}

void add_new_User(userList *list, int size, int ID)
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

    account* newAccount = new account;
    newAccount->IBAN = IBAN;
    newAccount->accountName = type;
    newAccount->balance = balance;
    newAccount->currency = currency;
    newAccount->limitDepositPerDay = limitDPD;
    newAccount->limitWithdrawPerMonth = limitWPM;
    newAccount->txn = NULL;
    newAccount->next = NULL;
   
    add_account_to_user(list, tmp, newAccount);
    accesschoice(list, size, ID);
    return;
}

// void access_transaction(){

// }

void accesschoice(userList *list, int size, int ID)
{
    char x;
    cout << endl
         << "\033[1;33m--> ~Press 1 to add a User .\n--> ~Press 2 to access the transaction menu.\n--> ~Press 0 to Return.\033[0m " << endl;
    do
    {
        cin >> x;
        if (x == '1')
        {
            cin.ignore();
            add_new_User(list, size, ID);
        }
        else if (x == '2')
        {
            cin.ignore();
            // access_transaction();
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
    accesschoice(list, size, ID);
    return;
}
void add_to_txt_file(userList *list) {
    fstream input;
    user *travel = list->head;
    int q = 0;
    input.open("input.txt", ios::out);
    
    while (travel != NULL) {
        if (q == 0) {
            input << "-" << travel->userID << "," << travel->fname << "," << travel->lname;
            q++;
        } else {
            input << endl << "-" << travel->userID << "," << travel->fname << "," << travel->lname;
        }

        account *tmp2 = travel->acct;
        while (tmp2 != NULL) {
            input << endl
                  << "#" << tmp2->IBAN << "," << tmp2->accountName << "," 
                  << tmp2->balance << tmp2->currency << "," 
                  << tmp2->limitDepositPerDay << tmp2->currency << "," 
                  << tmp2->limitWithdrawPerMonth << tmp2->currency;
         
            transaction *tmp3 = tmp2->txn;
            while(tmp3 != NULL) {
                input << endl << "*" << tmp3->date << "," << tmp3->amount;
                tmp3 = tmp3->next;
            }
            tmp2 = tmp2->next;
        }
        travel = travel->next;
    }
    input.close();
    return;
}

void choice(userList *c, int size)
{
    // print(c);
    char x;
    cout << "\033[1;33m--> ~Press 1 to access a account.\n--> ~Press 2 to add a account.\n--> ~Press 0 to exit.\033[0m " << endl;
    do
    {
        cin >> x;
        if (x == '2')
        {
            cin.ignore();
            AddNewUser(c, size);
        }
        else if (x == '1')
        {
            cin.ignore();
            access(c, size);
        }
        else if (x == '0')
        {
            add_to_txt_file(c);
            cout << "\033[30m-->Thanks for using our service.<--\033[0m\n";
        }
        else
        {
            cout << "\033[1;31m--> Invalid choice.\033[0m\n";
        }
    } while (x != '1' && x != '2' && x != '0');
    return;
}
int main()
{   
    userList* list=new userList;
    list->head=NULL;
    list->tail=NULL;
    list = readFromFile(list);
    int size = sizereaderusr();
    choice(list, size);
    //the print function is changing the format after it prints so it the lists wont work after entering it be care full change it later also fix the setw() in it so it can calculat the length
    print(list);
    delete list;
}