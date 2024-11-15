#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
// #include <ctime>
// #include <chrono>
// #include<ios>

// .csv this is for the exel file to go from one to another we make the parameter ','

// to get the current time
// time_t currentTime = time(NULL); 
// struct tm *localTime = localtime(&currentTime);
using namespace std;

struct transaction {
    string date;
    double amount;
    transaction *next;
};
struct account {
    string IBAN;
    string accountName;
    double balance;
    string currency;
    double limitDepositPerDay;
    double limitWithdrawPerMonth;
    // transaction *txn;
    account *next;
};
struct user {
    int userID;
    string fname;
    string lname;
    account *acct; 
    user *next, *previous; 
};
struct userList {
    user *head, *tail;
};

int sizereaderclients()
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

void AddNewUser(user * &firsttime,int &val){
    string first,last;
    user *q=new user;
    int x;
    do{
        x=0;
        cout<<"Please enter your First Name:  ";
        getline(cin,first);
        if(first.length() >= 1)
            x++;
        else
            cout << "\033[1;31m--> Please enter a valid name.\033[0m" << endl;
        
    }while(x==0);

    int z;
    do{
        z=0;
        cout<<"Please enter your Last Name:  ";
        getline(cin,last);
        if(last.length() >= 1)
            z++;
        else
            cout << "\033[1;31m--> Please enter a valid name.\033[0m" << endl;
        
    }while(z==0);
    // if(firsttime==NULL){
    //     firsttime->fname = first;
    //     firsttime->lname = last;
    //     firsttime->next=nullptr;
    //     firsttime->previous=nullptr;
    //     cout<<"fuck you";
    // }
    // else{
    //     while(firsttime!=NULL){
    //         firsttime->next;
    //     }
    //     firsttime->fname = first;
    //     firsttime->lname=last;
    // }
    

    return ;
}
void choice(user *c, int size){
    char x;
    cout << "\033[1;33m~Press 1 to login .\n~Press 2 to sighn up.\n~Press 0 to exit.\033[0m " << endl;
    do
    {
        cin >> x;
        if (x == '2')
        {
            cin.ignore();
            AddNewUser(c,size);
        }
        else if (x == '1')
        {
            // cin.ignore();
            // login(c, size);
        }
        else if (x == '0')
        {

            cout << "Thanks for using our service.";
            break;
        }
        else
        {
            cout
                << "\033[1;31m--> Invalid choice.\033[0m" << endl;
        }
    } while (x != '1' && x != '2' && x != '0');
    return;
}
int main(){
    user* head=new user;
    head=NULL;
    user*point =nullptr;
    int size=sizereaderclients();
    choice(point,size);
}