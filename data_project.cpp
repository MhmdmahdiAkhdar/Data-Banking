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


void AddNewUser(user *firsttime,int &val){
    user* first=new user;
    int x;
    do{
        x=0;
        cout<<"Please enter your First Name:  ";
        getline(cin,first->fname);
        if(first->fname.length() >= 1)
            x++;
        else
            cout << "\033[1;31m--> Please enter a valid name.\033[0m" << endl;
        
    }while(x==0);

    int z;
    do{
        z=0;
        cout<<"Please enter your Last Name:  ";
        getline(cin,first->lname);
        if(first->lname.length() >= 1)
            z++;
        else
            cout << "\033[1;31m--> Please enter a valid name.\033[0m" << endl;
        
    }while(z==0);
    
}

int main(){
    user* head;
    int n=0;
    AddNewUser(head,n);

}