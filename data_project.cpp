#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cctype>
// #include <ctime>
// #include <chrono>
// #include<ios>

// .csv this is for the exel file to go from one to another we make the parameter ','

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
    // transaction *txn;
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

void filltxt()
{
}

// prints every thing this may be deleted later on
void print(user *head)
{
    user *travel = head;
    while (travel != nullptr)
    {
        cout << travel->userID << "      " << travel->fname << "     " << travel->lname << endl;
        travel = travel->next;
    }
}

int sizereaderusr()
{
    fstream file;
    int size = 0;
    string line;
    file.open("clients.txt", ios::in);
    while (getline(file, line))
    {
        size++;
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

void AddNewUser(user *&node, int &val)
{
    string first, last;
    int x;
    x = 0;
    cout << "Please enter your First Name:  ";
    checkname(first);
    cout << "Please enter your Last Name:  ";
    checkname(last);

    user *head, *tail;
    if ((node->next == NULL) && (node->previous == NULL))
    {
        // inputs if the pointer is null
        cout << "ehwfwef";
        node->fname = first;
        node->lname = last;
        node->userID = val + 1;
        node->next = nullptr;
        node->previous = nullptr;
        head = node;
        tail = node;
    }
    else
    {
        user *travel;
        travel = node;
        // this input is just for the second value
        while (travel->next != NULL)
        {
            cout << "fuck you";
            travel = travel->next;
        }
        travel->fname = first;
        travel->lname = last;
        travel->userID = val + 1;
        travel->next = nullptr;
        travel->previous = tail;
        tail->next = travel;
        tail = travel;
        head = travel;
    }

    cout << "\033[1;32mAn account has been created....\033[0m\n";
    choice(node, val + 1);
    print(node);
    return;
}
void choice(user *c, int size)
{
    char x;
    cout << "\033[1;33m~Press 1 to login .\n~Press 2 to sighn up.\n~Press 0 to exit.\033[0m " << endl;
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
            // cin.ignore();
            // login(c, size);
        }
        else if (x == '0')
        {

            cout << "Thanks for using our service.\n";
            break;
        }
        else
        {
            cout
                << "\033[1;31m--> Invalid choice.\033[0m\n";
        }
    } while (x != '1' && x != '2' && x != '0');
    return;
}
int main()
{
    user *node = new user();
    node->next = nullptr;
    node->previous = nullptr;
    int size = sizereaderusr();
    choice(node, size);
}