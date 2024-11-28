#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cctype>
#include<cmath>
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

void choice(user *, int);
void access(user *, int);
void accesschoice(user *, int, int);
void print(user *);
user *readfromfile(user *&head)
{
    ifstream input;
    input.open("input.txt", ios::in);
    string line;
    user *tail = NULL;
    while (getline(input, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '-')
        {
            line = line.substr(1);
            //  else continue;

            stringstream ss(line);
            string userIDStr, fname, lname;

            getline(ss, userIDStr, ',');
            getline(ss, fname, ',');
            getline(ss, lname, ',');

            user *newUser = new user;
            newUser->userID = stoi(userIDStr);
            newUser->fname = fname;
            newUser->lname = lname;
            newUser->next = NULL;
            newUser->acct = NULL;
            if (head == NULL)
            {
                head = newUser;
                tail = head;
            }
            else
            {
                tail->next = newUser;
                tail = newUser;
            }
            // tail->acct=NULL;
        }
        else if (line[0] == '#')
        {
            line = line.substr(1);
            stringstream ss(line);
            string IBAN, accName, currency, balandcur, limitPD , limitPDandcurr , limitPM , limitPMandcurr , balance;
            getline(ss, IBAN, ',');
            getline(ss, accName, ',');
            getline(ss, balandcur, ',');
            getline(ss, limitPDandcurr, ',');
            getline(ss, limitPMandcurr, ',');
            // this function is in the string library that finds the first non present element and return its position
            int pos = balandcur.find_first_not_of("0123456789.");
            balance = balandcur.substr(0, pos);
            currency = balandcur.substr(pos);
            pos = limitPDandcurr.find_first_not_of("0123456789.");
            limitPD = limitPDandcurr.substr(0, pos);
            pos = limitPMandcurr.find_first_not_of("0123456789.");
            limitPM = limitPMandcurr.substr(0, pos);

            account *newAccount = new account;
            newAccount->IBAN = IBAN;
            newAccount->accountName = accName;
            newAccount->balance = stod(balance);
            newAccount->currency = currency;
            newAccount->limitDepositPerDay = stod(limitPD);
            newAccount->limitWithdrawPerMonth = stod(limitPM);
            newAccount->txn = NULL;
            newAccount->next = NULL;
            cout<<newAccount->balance<<endl;
            account *tmp = tail->acct;
            if (tmp == NULL)
            {
                tmp = newAccount;
                tail->acct = newAccount;
            }
            else
            {
                while (tmp->next != NULL)
                {
                    tmp = tmp->next;
                }
                tmp->next = newAccount;
            }
        }
    }
    input.close();
    return head;
}

// prints every thing this may be deleted later on
void print(user *head)
{
    user *travel = head;
    while (travel != NULL)
    {
        cout << "-" << travel->userID << setw(20) << travel->fname << setw(20) << travel->lname << endl;
        while (travel->acct != NULL)
        {
            cout << "#" << travel->acct->IBAN << "," << travel->acct->accountName << "," << travel->acct->balance << travel->acct->currency << "," << travel->acct->limitDepositPerDay << "," << travel->acct->limitWithdrawPerMonth << endl;
            travel->acct = travel->acct->next;
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
user *insertAtEnd(user *head, string first, string last)
{
    user *tmp = new user; // creates a new pointer
    tmp->fname = first;
    tmp->lname = last;
    tmp->next = NULL; // in the last node the next is allways null
    tmp->acct = NULL;
    if (head == NULL)
    {                    // if there is no other pointers
        tmp->userID = 1; // Assign first user ID
        tmp->previous = NULL;
        return tmp; // New head
    }

    user *cur = head;
    while (cur->next != NULL)
    {
        cur = cur->next; // Traverse to the end of the list
    }

    cur->next = tmp;               // Link the new node
    tmp->previous = cur;           // Set the previous pointer
    tmp->userID = cur->userID + 1; // Increment user ID
    return head;                   // Return the head of the list
}
bool repetion(user *head, string first, string last)
{
    user *travel = head;
    while (travel != nullptr)
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
void AddNewUser(user *&node, int &val)
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
        x = repetion(node, first, last);
        if (x == true)
        {
            q = allocation_input();
            if (q == "1")
            {
                access(node, val);
                break;
            }
        }
    } while (x == true);
    if (q == "2")
    {
        node = insertAtEnd(node, first, last);
        cout << "\033[1;32m--> An account has been created....\nYour ID is: " << val << "\033[0m\n";
        choice(node, val);
    }
    return;
}
user *checklogin(user *head, string first, string last, int ID)
{
    user *travel = head;
    while (travel != nullptr)
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
        // for (char c : IBAN)
        // {
        //     if (!isdigit(c))
        //     {
        //         cout << "\033[1;31m--> You can enter only numbers.\n \033[0m";
        //         x = false;
        //         break;
        //     }
        //     else
        //         x = true;
        // }
    } while (!isUnique );
    return;
}

account *add_new_account_input(account *tmpaccounts, string IBAN, string type, string currency, double balance, double limitDPD, double limitWPM)
{
    account *tmp = tmpaccounts;
    account *newAccount = new account;
    newAccount->IBAN = IBAN;
    newAccount->accountName = type;
    newAccount->currency = currency;
    newAccount->balance = balance;
    newAccount->limitDepositPerDay = limitDPD;
    newAccount->limitWithdrawPerMonth = limitWPM;
    newAccount->txn = NULL;
    newAccount->next = NULL;
    if (tmp == NULL)
    {
        return newAccount;
    }
    else
    {
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = newAccount;
        return tmpaccounts;
    }
}

void add_new_User(user *node, int size, int ID)
{
    user *tmp = node;
    while (tmp->userID != ID && tmp != NULL)
    {
        tmp->previous = tmp;
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
    tmp->acct = add_new_account_input(tmp->acct, IBAN, type, currency, balance, limitDPD, limitWPM);
    accesschoice(node, size, ID);
    return;
}

void access_transaction(){

}

void accesschoice(user *node, int size, int ID)
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
            add_new_User(node, size, ID);
        }
        else if (x == '2')
        {
            cin.ignore();
            // access(c, size);
        }
        else if (x == '0')
        {
            choice(node, size);
            return;
            break;
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
void access(user *node, int size)
{
    int ID;
    user *chosen;
    do
    {
        string firstname, lastname;
        ID_checker(ID);
        cout << "\033[1;33m--> Eenter your first name: \033[0m";
        checkname(firstname);
        cout << "\033[1;33m--> Eenter your last name: \033[0m";
        checkname(lastname);
        chosen = checklogin(node, firstname, lastname, ID);
    } while (chosen == NULL);
    accesschoice(node, size, ID);
    return;
}
void add_to_txt_file(user *node)
{
    fstream input;
    // ofstream input;
    user *travel = node;
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
                  << "#" << tmp2->IBAN << "," << tmp2->accountName << "," << tmp2->balance << tmp2->currency << "," << tmp2->limitDepositPerDay<< tmp2->currency << "," << tmp2->limitWithdrawPerMonth<< tmp2->currency;
         
            transaction *tmp3=tmp2->txn;
            while(tmp3!=NULL){
                input<<endl<<"*"<<tmp3->date<<","<<tmp3->amount;
                tmp3=tmp3->next;
            }
            tmp2 = tmp2->next;
        }
        travel = travel->next;
    }
    input.close();
    return;
}

void choice(user *c, int size)
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
            // break;
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
    userList* nodes=new userList;
    nodes->head=NULL;
    user *node = new user;
    node = NULL;
    node = readfromfile(node);
    int size = sizereaderusr();
    choice(node, size);
    //the print function is changing the format after it prints so it the lists wont work after entering it be care full change it later also fix the setw() in it so it can calculat the length
    print(node);
    delete node;
}