#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <iomanip>
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
void access(user *, int);

void readfromfile(user *&head)
{
    ifstream input;
    input.open("input.txt", ios::in);
    string line;
    user *tail = nullptr;
    while (getline(input, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '-')
            line = line.substr(1);

        stringstream ss(line);
        string userIDStr, fname, lname;

        getline(ss, userIDStr, ',');
        getline(ss, fname, ',');
        getline(ss, lname, ',');

        user *newUser = new user();
        newUser->userID = stoi(userIDStr);
        newUser->fname = fname;
        newUser->lname = lname;
        newUser->next = nullptr;

        if (head == nullptr)
        {
            head = newUser;
            tail = head;
        }
        else
        {
            tail->next = newUser;
            tail = newUser;
        }
    }

    input.close();
}

// prints every thing this may be deleted later on
void print(user *head)
{
    user *travel = head;
    while (travel != nullptr)
    {
        cout << travel->userID << setw(20) << travel->fname << setw(20) << travel->lname << endl;
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
user *insertAtEnd(user *head, string first, string last)
{
    user *tmp = new user; // creates a new pointer
    tmp->fname = first;
    tmp->lname = last;
    tmp->next = nullptr; // in the last node the next is allways null

    if (head == nullptr)
    {                    // if there is no other pointers
        tmp->userID = 1; // Assign first user ID
        tmp->previous = nullptr;
        return tmp; // New head
    }

    user *cur = head;
    while (cur->next != nullptr)
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
            cout << "\033[1;31mThere is an account with this name.\033[0m\n";
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
    val = val + 1;
    string q = "2";
    do
    {
        cout << "Please enter your First Name:  ";
        checkname(first);
        cout << "Please enter your Last Name:  ";
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
        cout << "\033[1;32mAn account has been created....\nYour ID is: " << val << "\033[0m\n";
        choice(node, val);
    }
    return;
}
bool checklogin(user *head, string first, string last, int ID)
{
    user *travel = head;
    while (travel != nullptr)
    {
        if (travel->fname == first && travel->lname == last && travel->userID == ID)
        {
            return true;
        }
        travel = travel->next;
    }
    cout << "\033[1;31mThere is no account with this information.\033[0m\n";
    return false;
}

void accesschoice(user *node,int size){
     char x;
    cout << "\033[1;33m~Press 1 to add account .\n~Press 2 to do a transaction.\n~Press 0 to Return.\033[0m " << endl;
    do
    {
        cin >> x;
        if (x == '2')
        {
            cin.ignore();
            // AddNewUser(c, size);
        }
        else if (x == '1')
        {
            cin.ignore();
            // access(c, size);
        }
        else if (x == '0')
        {
            choice(node,size);
            break;
        }
        else
        {
            cout<< "\033[1;31m--> Invalid choice.\033[0m\n";
        }
    } while (x != '1' && x != '2' && x != '0');
    return;
}

void access(user *node, int size)
{
    int ID;
    bool x;
    do
    {
        string firstname, lastname;
        cout << "Please enter your ID: ";
        cin >> ID;
        cin.ignore();
        cout << "Please enter your first name: ";
        checkname(firstname);
        cout << "Please enter your last name: ";
        checkname(lastname);
        x = checklogin(node, firstname, lastname, ID);
    } while (x == false);
    return;
        x=checklogin(node,firstname,lastname,ID);
    }while(x==false);
    accesschoice(node,size);
    return ;
}
void add_to_txt_file(user *node, int size)
{
    fstream input;
    user *travel = node;
    int q = 0;
    // if(size-1==0){
    input.open("input.txt", ios::out);
    while (travel != nullptr)
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
        travel = travel->next;
    }
    input.close();
    // }
    // else{
    //     input.open("input.txt", ios::app);
    //     while(travel!=nullptr){
    //             input <<endl<<"-"<< travel->userID << "," << travel->fname << "," << travel->lname;
    //             travel = travel->next;
    //     }
    //     input.close();
    // }
}

void choice(user *c, int size)
{
    char x;
    cout << "\033[1;33m~Press 1 to access a user's account .\n~Press 2 to add a account.\n~Press 0 to exit.\033[0m " << endl;
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
            print(c);
            add_to_txt_file(c, size);
            cout << "\033[30mThanks for using our service.\033[0m\n";

            break;
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
    user *node = new user();
    node = NULL;
    readfromfile(node);
    int size = sizereaderusr();
    choice(node, size);
}