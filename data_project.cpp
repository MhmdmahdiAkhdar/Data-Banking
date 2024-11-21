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
void accesschoice(user *,int ,int);

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
    while (travel != NULL)
    {
        cout << travel->userID << setw(20) << travel->fname << setw(20) << travel->lname << endl;
        account *tmp=travel->acct;
        while(tmp!=NULL){
            cout<<tmp->IBAN<<setw(10)<<tmp->accountName<<setw(10)
            <<tmp->balance<<setw(10)<<tmp->limitDepositPerDay<<setw(10)<<tmp->limitWithdrawPerMonth
            <<setw(10)<<tmp->balance<<endl;
            tmp=tmp->next;
        }
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
    val = val + 1;
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
        travel = travel->next;
    }
    cout << "\033[1;31m--> There is no account with this information.\033[0m\n";
    return NULL;
}

void IBAN_confirm(account * node,string &IBAN){
    bool x;
    account *tmp=node;
    do{
        x=true;
    cout << "\033[1;33m-->Enter the IBAN of the new account: \033[0m";
    getline(cin,IBAN);
    while(tmp!=NULL){
        tmp->IBAN;
        if(IBAN==tmp->IBAN){
            cout<< "\033[1;31m--> This account has this IBNA please use a different one:\033[0m";
            getline(cin,IBAN);
            x= false;
        }
    }
    }while(x==false);
    return;
}
void account_type(string &type){
    bool x;
    string s;
    do{
        x=true;
    cout << "\033[1;33m-->Press 1 if the account is 'international'\n-->Press 2 if the account is 'domiciled'\033[0m\n";
    getline(cin,s);
    if(s.length()!=1){
        cout<< "\033[1;31m--> Invalid choice.\033[0m\n";
        x=false;
    }
    if(s=="1"){
        type="international";
    }
    else if(s=="2"){
        type="domiciled";
    }
    else x=false;
    }while(x==false);
    return;
}

void Deposit_check(double &balance){
    string blc;
    bool x;
    int z,y;
    do{
        z=y=0;
        getline(cin,blc);
        for(int i=0;i<blc.length();i++){
            if(blc[i]=='.'){
                z++;
            }
            if(isdigit(blc[i])){
                y++;
            }
        }
        if((z==1||z==0)&&y>0){
            x=true;
        }
        else x=false;
        if(x==false){
            cout<< "\033[1;31m--> Invalid choice.\033[0m\n\033[1;32m-->Enter the correct form:  \033[0m";
        }
    }while(x==false);
    balance=stod(blc);
    return;
}
account * add_account_after_get(account *node,int ID,string IBAN,string currency,string type,double balance,double limitDpD,double limitWPD){
    account *tmp2=node;
    account *info=new account;
    info->next=NULL;
    info->IBAN=IBAN;
    info->balance=balance;
    info->currency=currency;
    info->limitDepositPerDay=limitDpD;
    info->limitWithdrawPerMonth;
    info->accountName=type;
    if(tmp2==NULL){
        return info;
    }
    else{
    while(tmp2->next!=NULL){//if its next is null then the tmp2 has data
        tmp2=tmp2->next;
    }
    tmp2->next=info;
    return node;
    }
}
user * get_account(user *node,int ID,string IBAN,string currency,string type,double balance,double limitDpD,double limitWPD){
    user *tmp=node;
    while(tmp!=NULL){
        if(tmp->userID==ID){
            tmp->acct=add_account_after_get(tmp->acct,ID,IBAN,currency,type,balance,limitDpD,limitWPD);
            break;
        }
        tmp=tmp->next;   
    }
    return node;
}


//chat gpt code
// account* add_account_after_get(account* node, string IBAN, string currency, string type, 
//                                double balance, double limitDpD, double limitWPD) {
//     account* tmp2 = node;
//     account* info = new account;

//     // Initialize the new account node
//     info->IBAN = IBAN;
//     info->balance = balance;
//     info->currency = currency;
//     info->limitDepositPerDay = limitDpD;
//     info->limitWithdrawPerMonth = limitWPD;
//     info->accountName = type;
//     info->next = NULL;

//     // If the account list is empty, return the new account node
//     if (tmp2 == NULL) {
//         return info;
//     }

//     // Traverse to the end of the linked list
//     while (tmp2->next != NULL) {
//         tmp2 = tmp2->next;
//     }

//     // Add the new account to the end of the list
//     tmp2->next = info;

//     return node;
// }

// user* get_account(user* node, int ID, string IBAN, string currency, string type, 
//                   double balance, double limitDpD, double limitWPD) {
//     user* tmp = node;

//     // Traverse the user linked list
//     while (tmp != NULL) {
//         if (tmp->userID == ID) {
//             // Add the account to the matching user's account list
//             tmp->acct = add_account_after_get(tmp->acct, IBAN, currency, type, 
//                                               balance, limitDpD, limitWPD);
//             break;
//         }
//         tmp = tmp->next;
//     }

//     return node;
// }

//note this is experimental its still not working
void add_new_account_input(user *node,int size,int ID){
    string IBAN,type,currency;
    double balance,limitDPD,limitWPD;
    IBAN_confirm(node->acct,IBAN);
    // getline(cin,IBAN);
    account_type(type);
    // getline(cin,type);
    cout << "\033[1;33mEnter the currency of the new account(example: $): \033[0m";
    //confirmation sal assaf as for now the currency wont be checked
    getline(cin,currency);
    cout << "\033[1;33mEnter the Balance of the new account: \033[0m";
    Deposit_check(balance);
    cout << "\033[1;33mEnter the  limit Deposit PerDay of the new account: \033[0m";
    Deposit_check(limitDPD);
    cout << "\033[1;33mEnter the  limit Withdraw PerMonth of the new account: \033[0m";
    Deposit_check(limitWPD);
    //this function has a problem test it ??-->only inputs one function
    // node=get_account(node,ID,IBAN,currency,type,balance,limitDPD,limitWPD);
    accesschoice(node,size,ID);
    return;
}

void accesschoice(user *node,int size,int ID){
     char x;
    cout <<endl<< "\033[1;33m--> ~Press 1 to add an account .\n--> ~Press 2 to do a transaction.\n--> ~Press 0 to Return.\033[0m " << endl;
    do
    {
        cin >> x;
        if (x == '1')
        {
            print(node);
            cin.ignore();
            add_new_account_input(node,size,ID);
        }
        else if (x == '2')
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

void ID_checker(int &ID)
{
    string id;
    bool x;
    do{
    cout << "\033[1;33m-->Enter your ID: \033[0m";
    getline(cin,id);
    for (char c : id)
        {
            if (!isdigit(c))
            {
                x = false;
                break;
            }
            else x= true;
        }
    }while(x==false);
    ID=stoi(id);
}
void access(user *node, int size)
{
    int ID;
    user *chosen;
    do
    {
        string firstname, lastname;
        ID_checker(ID);
        cout << "\033[1;33--> Eenter your first name: \033[0m";
        checkname(firstname);
        cout << "\033[1;33--> Eenter your last name: \033[0m";
        checkname(lastname);
        chosen = checklogin(node, firstname, lastname, ID);
    } while (chosen == NULL);
    accesschoice(chosen,size,ID);
    return ;
}
void add_to_txt_file(user *node)
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
    cout << "\033[1;33m--> ~Press 1 to access a user.\n--> ~Press 2 to add a account.\n--> ~Press 0 to exit.\033[0m " << endl;
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
            add_to_txt_file(c);
            cout << "\033[30m-->Thanks for using our services<--\033[0m\n";

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
    user *node = new user;
    node = NULL;
    readfromfile(node);
    int size = sizereaderusr();
    choice(node, size);
    delete node;
}