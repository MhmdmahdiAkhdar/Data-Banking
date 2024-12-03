#include <iostream>
#include <string>
#include <ios>
#include <sstream>
// #include "sha1.h"
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <unordered_set>
// #include "hpdf.h"
using namespace std;
time_t currentTime = time(NULL); // to get the current time
struct tm *localTime = localtime(&currentTime);
struct date
{
    string sdate, edate;
};
struct car
{
    string paltenumber;
    string Brand;
    string model;
    int year;
    string color;
    double pricePerDaye;
    date d;
};
struct client
{
    int userid;
    string firstname, lastname, password, phonenumber, email;
    int nbreservation;
    car *c;
};
// calling of functions neaded
void admininterface();
void login(client *, int);
void choice(client *, int);
void plaitnumbercheck(string &);
void brandcheck(string &);
int yearcheck(string);
void modelcheck(string &);
void colorcheck(string &);
double pricecheck(string);
void login(client *, int);
void userinterface(client *, int, int);
void displayuserrental(client *, int, int);
void sort(car *, int, double *);
void array_rented(client *, int);
void fill_pdf(client *, int);

// displays the rented cars the user has
void displaytheuserrent(client *cl, int user)
{
    int maxPlateNumberWidth = 0, wedth_sdate = 0, wedth_edate = 0;
    for (int i = 0; i < cl[user].nbreservation; i++)
    {

        if (cl[user].c[i].paltenumber.length() > maxPlateNumberWidth)
        {
            maxPlateNumberWidth = cl[user].c[i].paltenumber.length();
        }

        if (cl[user].c[i].d.sdate.length() > wedth_sdate)
        {
            wedth_sdate = cl[user].c[i].d.sdate.length();
        }
        if (cl[user].c[i].d.edate.length() > wedth_edate)
        {
            wedth_edate = cl[user].c[i].d.edate.length();
        }
    }
    string pl = "Plate Number", sd = "start date", ed = "end date";
    if (pl.length() > maxPlateNumberWidth)
    {
        maxPlateNumberWidth = pl.length();
    }
    if (sd.length() > wedth_sdate)
    {
        wedth_sdate = sd.length();
    }
    if (sd.length() > wedth_edate)
    {
        wedth_edate = ed.length();
    }
    // Display the data with proper spacing
    cout << left << setw(maxPlateNumberWidth + 2) << "Plate Number"
         << setw(wedth_sdate + 2) << "Start date "
         << setw(wedth_edate + 2) << "End date " << endl;

    for (int i = 0; i < cl[user].nbreservation; i++)
    {
        cout << left << setw(maxPlateNumberWidth + 2) << cl[user].c[i].paltenumber
             << setw(wedth_sdate + 2) << cl[user].c[i].d.sdate
             << setw(wedth_edate + 2) << cl[user].c[i].d.edate << endl;
    }
}
// reads size of the clientsdata.csv file
int sizereaderclients(fstream &file)
{
    int size = 0;
    string line;
    file.open("clients.csv", ios::in);
    while (getline(file, line))
    {
        size++;
    }
    file.close();
    return size;
}
//  reads the size from car data
int sizereadercars(fstream &cars)
{
    cars.open("cardata.csv", ios::in);
    int size = 0;
    string line;
    while (getline(cars, line))
    {
        size++;
    }
    cars.close();
    return size;
}
// read the size from the rentedcars file
int sizereaderrentedcars(fstream &rentedcars)
{
    rentedcars.open("rentedcars.csv", ios::in);
    int size = 0;
    string line;
    while (getline(rentedcars, line))
    {
        size++;
    }
    rentedcars.close();
    return size;
}
// to check the phone number
bool phonenbcheck(string i)
{
    {
        if (i.at(0) == '7')
        {
            if (i.at(1) == '1' || i.at(1) == '0' || i.at(1) == '6' || i.at(1) == '8')
                if (i.at(2) == '-')
                    return true;
        }
        else if (i.at(0) == '0')
        {
            if (i.at(1) == '3' || i.at(1) == '1')
                if (i.at(2) == '-')
                    return true;
        }
        else if (i.at(0) == '8')
        {
            if (i.at(1) == '1' || i.at(1) == '6' || i.at(1) == '3')
                if (i.at(2) == '-')
                    return true;
        }
        return false;
    }
}
// checks if it contains only numbers in phone
bool onlynumber(string i)
{
    int ff = 0;
    for (int j = 0; j < i.length(); j++)
    {
        if (i.at(j) >= 48 && i.at(j) <= 57 || i.at(2) == '-')
            ff++;
    }
    if (ff == 9)
        return true;
    return false;
}
// checks if phone number is already in the file
bool phonecheck(client *c, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (c[i].phonenumber == c[size - 1].phonenumber)
        {
            return false;
        }
    }
    return true;
}
// inputs the client's phone number and checks if it is valid
void phonenumber(string &phonenumber)
{
    bool temp = true;
    do
    {
        temp = true;
        cout << "Phone number (ex: 71-203040) : ";
        getline(cin, phonenumber);
        if (phonenumber.length() != 9)
        {
            cout << "\033[1;31mInvalid phone number , or phone already exists .\033[0m\n";
            temp = false;
        }
        else if (!phonenbcheck(phonenumber))
        {
            cout << "\033[1;31mInvalid phone number , or phone already exists .\033[0m\n";

            temp = false;
        }
        else if (!onlynumber(phonenumber))
        {
            cout << "\033[1;31mInvalid phone number , or phone already exists .\033[0m\n";

            temp = false;
        }
    } while (!temp);
}
// password  input and check
void password(string &password)
{

    int *p = new int[4];
    do
    {
        for (int i = 0; i < 4; i++)
        {
            *(p + i) = 0;
        }
        cout << "password : ";
        getline(cin, password);
        if (password.length() >= 8)
            *(p + 0) = 1;
        for (int j = 0; j < password.length(); j++)
        {
            if (password[j] >= 48 && password[j] <= 57)
                *(p + 1) = 1;
            else if (password[j] >= 97 && password[j] <= 122 || password[j] >= 65 && password[j] <= 90)
                *(p + 2) = 1;
            else if (password[j] >= 33 && password[j] <= 47 || password[j] >= 58 && password[j] <= 64 || password[j] >= 91 && password[j] <= 96 || password[j] >= 123 && password[j] <= 126)
                *(p + 3) = 1;
        }
        if (*(p + 0) == 0)
            cout << "\033[1;31m--> The paswword length should be at least 8!\033[0m " << endl;
        if (*(p + 1) == 0)
            cout << "\033[1;31m--> The password needs numbers!\033[0m " << endl;
        if (*(p + 2) == 0)
            cout << "\033[1;31m--> The password needs letters!\033[0m" << endl;
        if (*(p + 3) == 0)
            cout << "\033[1;31m--> The password needs special charachter!\033[0m" << endl;
    } while (*(p + 3) == 0 || *(p + 2) == 0 || *(p + 1) == 0 || *(p + 0) == 0);
}
// cheks if end is .com or edu.lb
bool emailend(string Email)
{
    for (int i = 0; i < Email.length(); i++)
    {
        if (Email[i] == '.')
        {
            if (Email[i + 1] == 'c' && Email[i + 2] == 'o' && Email[i + 3] == 'm' && Email[i + 4] == '\0')
            {
                return true;
            }
            else if (Email[i + 1] == 'e' && Email[i + 2] == 'd' && Email[i + 3] == 'u' && Email[i + 4] == '.' && Email[i + 5] == 'l' && Email[i + 6] == 'b' && Email[i + 7] == '\0')
            {
                return true;
            }
        }
    }
    return false;
}
// cheks if email have  1 @ and at lest one .
bool emailcount(string Email)
{
    int at = 0, dot = 0;
    for (int i = 0; i < Email.length(); i++)
    {
        if (Email[i] == '@')
        {
            at++;
        }
        if (Email[i] == '.')
        {
            dot++;
        }
    }
    if (at == 1 && dot >= 1)
    {
        return true;
    }
    return false;
}
// checks it there is a letter after @
bool afterat(string Email)
{
    for (int i = 0; i < Email.length(); i++)
    {
        if (Email[i] == '@')
        {
            if (Email[i + 1] >= 'a' && Email[i + 1] <= 'z')
            {
                return true;
            }
        }
    }
    return false;
}
// checks if email is already used by serching the file if not repeted returns true
bool emailreption(client *c, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (c[i].email == c[size - 1].email)
        {
            return false;
        }
    }
    return true;
}
// cheks if email starts with a letter
bool emailstart(string Email)
{
    if ((Email[0] >= 'A' && Email[0] <= 'Z') || (Email[0] >= 'a' && Email[0] <= 'z'))
    {
        return true;
    }
    return false;
}
// enters the clients email
void email(string &email)
{
    bool *p = new bool[5];
    do
    {
        for (int i = 0; i < 5; i++)
        {
            *(p + i) = false;
        }
        cout << "Email : ";
        getline(cin, email);
        if (emailend(email) == true)
        {
            *(p + 0) = true;
        }
        if (emailcount(email) == true)
        {
            *(p + 1) = true;
        }
        if (emailstart(email) == true)
        {
            *(p + 2) = true;
        }
        if (afterat(email) == true)
        {
            *(p + 3) = true;
        }
        if (email.length() > 1)
        {
            *(p + 4) = true;
        }

        else
            cout << "\033[1;31mThe email you entred is used before try again\033[0m\n";

        if (*(p + 0) == 0 || *(p + 1) == 0 || *(p + 2) == 0 || *(p + 3) == 0 || *(p + 4) == 0)
            cout << "\033[1;31m--> Invalid Email.\033[0m \n";

    } while (*(p + 0) == false || *(p + 1) == false || *(p + 2) == false || *(p + 3) == false || *(p + 4) == false);
}
// adds the new clients data to the file
void addclient(client *c, int size)
{
    fstream client;
    if (size - 1 == 0)
    {
        client.open("clients.csv", ios::out);

        client << c[0].userid << ',' << c[0].firstname << ',' << c[0].lastname << ',' << c[0].password << ',' << c[0].email << ',' << c[0].phonenumber;
        client.close();
    }
    else
    {
        client.open("clients.csv", ios::app);
        client << endl
               << c[size - 1].userid << ',' << c[size - 1].firstname << ',' << c[size - 1].lastname << ',' << c[size - 1].password << ',' << c[size - 1].email << ',' << c[size - 1].phonenumber;
        client.close();
    }
}
// clear existing data in cardata and place it again used for remove and modi
void fillcardata_modify_delete(car *cr, fstream &cardata, int size)
{
    cardata.open("cardata.csv", ios::out);
    cardata << cr[0].paltenumber << ',' << cr[0].Brand << ',' << cr[0].year << ',' << cr[0].model << ',' << cr[0].pricePerDaye << '$' << ',' << cr[0].color;
    cardata.close();
    cardata.open("cardata.csv", ios::app);
    for (int i = 1; i < size; i++)
    {
        cardata << "\n"
                << cr[i].paltenumber << ',' << cr[i].Brand << ',' << cr[i].year << ',' << cr[i].model << ',' << cr[i].pricePerDaye << '$' << ',' << cr[i].color;
    }
    cardata.close();
}
// removes car from adminstrator
void removecar(car *cr, fstream &cardata, int size)
{
    string platenumber;
    int n;
    int sizee = size - 1;
    car *t = new car[sizee];
    do
    {
        n = -1;
        cin.ignore();
        plaitnumbercheck(platenumber);
        for (int i = 0; i < size; i++)
        {
            if (cr[i].paltenumber == platenumber)
            {
                n = i;
                break;
            }
        }

        if (n == -1)
        {
            cout << "\033[1;31mThis plait number dose not exist.\033[0m";
        }
        else
        {
            for (int i = n; i < sizee; i++)
            {
                cr[i].paltenumber = cr[i + 1].paltenumber;
                cr[i].Brand = cr[i + 1].Brand;
                cr[i].year = cr[i + 1].year;
                cr[i].model = cr[i + 1].model;
                cr[i].pricePerDaye = cr[i + 1].pricePerDaye;
                cr[i].color = cr[i + 1].color;
            }
            for (int i = 0; i < sizee; i++)
            {
                t[i].paltenumber = cr[i].paltenumber;
                t[i].Brand = cr[i].Brand;
                t[i].year = cr[i].year;
                t[i].model = cr[i].model;
                t[i].pricePerDaye = cr[i].pricePerDaye;
                t[i].color = cr[i].color;
            }
            delete[] cr;
            cr = t;
        }
    } while (n == -1);
    fillcardata_modify_delete(cr, cardata, sizee);
    admininterface();
    return;
}
// to modify the car data in the  array
void modifycar(car *cr, int size)
{
    int n = -1;
    char x;
    int year;
    string z;
    fstream cardata;
    string platenumber;
    string choice;
    cout << "Enter the car Plate Number of the car you want to modify." << endl;
    cin.ignore();
    plaitnumbercheck(platenumber);
    do
    {
        for (int i = 0; i < size; i++)
        {
            if (cr[i].paltenumber == platenumber)
                n = i;
        }
        if (n == -1)
        {
            cout << "\033[1;31mThere is no car with that plate number !\033[0m" << endl;
        }
        else
        {
            cout << "What do you need to modify in this car?\n1-Plate Number: " << cr[n].paltenumber << "\n2-Brand: " << cr[n].Brand << "\n3-Year: " << cr[n].year

                 << "\n4-Model: " << cr[n].model << "\n5-Price : " << cr[n].pricePerDaye << '$' << "\n6-Color : " << cr[n].color << "\nOr tab 0 to exit." << endl;
            cin >> x;
            cin.ignore();
            if (x == '1')
            {
                plaitnumbercheck(choice);
                cr[n].paltenumber = choice;
                break;
            }
            else if (x == '2')
            {
                brandcheck(choice);
                cr[n].Brand = choice;
                break;
            }
            else if (x == '3')
            {
                cr[n].year = yearcheck(choice);
                break;
            }
            else if (x == '4')
            {
                modelcheck(choice);
                cr[n].model = choice;
                break;
            }
            else if (x == '5')
            {
                cr[n].pricePerDaye = pricecheck(z);
                break;
            }
            else if (x == '6')
            {
                colorcheck(choice);
                cr[n].color = choice;
                break;
            }
            else if (x == '0')
            {
                return;
            }
        }
        if (n == -1 || x != '1' || x != '2' || x != '3' || x != '4' || x != '5' || x != '6' || x != '0')
        {
            cout << "Invalid choice." << endl;
        }

    } while (n == -1 || x != '1' || x != '2' || x != '3' || x != '4' || x != '5' || x != '6' || x != '0');
    fillcardata_modify_delete(cr, cardata, size);
    admininterface();
    return;
}
// adds the new clients data to the file
void addcar(car *cr, int size)
{
    fstream client;
    if (size - 1 == 0)
    {
        client.open("cardata.csv", ios::out);

        client << cr[0].paltenumber << ',' << cr[0].Brand << ',' << cr[0].year << ',' << cr[0].model << ',' << cr[0].pricePerDaye << '$' << ',' << cr[0].color;
        client.close();
    }
    else
    {
        client.open("cardata.csv", ios::app);
        client << endl
               << cr[size - 1].paltenumber << ',' << cr[size - 1].Brand << ',' << cr[size - 1].year << ',' << cr[size - 1].model << ',' << cr[size - 1].pricePerDaye << '$' << ',' << cr[size - 1].color;
        client.close();
    }
}
// add the first and last name
void FNLN(string &Firstname, string &lastname)
{
    bool z, k;
    do
    {
        cout << "First name : ";
        z = false;
        getline(cin, Firstname);
        if (Firstname.length() >= 1)
            z = true;
        else
            cout << "\033[1;31m--> Please enter a valid name.\033[0m" << endl;

    } while (z == false);

    do
    {
        k = false;
        cout << "Last name : ";
        getline(cin, lastname);
        if (lastname.length() >= 1)
            k = true;
        else
            cout << "\033[1;31m--> Please enter a valid name.\033[0m" << endl;

    } while (k == false);
}
// sighnup function then continue to login
void sighnup(client *c, int size)
{
    int sizee = size + 1;
    client *t = new client[sizee];
    for (int i = 0; i < size; i++)
    {
        t[i].email = c[i].email;
        t[i].password = c[i].password;
        t[i].phonenumber = c[i].phonenumber;
        t[i].userid = c[i].userid;
    }
    delete[] c;
    client *i = new client;
    c = t;
    FNLN(c[sizee - 1].firstname, c[sizee - 1].lastname);
    do
    {
        email(c[sizee - 1].email);
        if (!emailreption(c, sizee))
        {
            cout << "This email exits\n";
        }
    } while (!emailreption(c, sizee));
    password(c[sizee - 1].password);
    // c[sizee - 1].password = sha1(c[sizee - 1].password);
    do
    {
        phonenumber(c[sizee - 1].phonenumber);
        if (!phonecheck(c, sizee))
        {
            cout << "This phone number exits";
        }
    } while (!phonecheck(c, sizee));
    c[sizee - 1].userid = size + 1;
    cout << "your user id is " << c[sizee - 1].userid << endl;
    addclient(c, sizee);
    cout << "Enter the data you entered befor to login : " << endl;
    login(c, sizee);
    return;
}
// inputs what is in the file inside the dynamic allocation if (size>=1)else no inut no clients
void fillarayclients(fstream &file, client *c, int size)
{

    file.open("clients.csv", ios::in);
    string line, s;
    int dest = 0, pos = 0;
    int sizec = 0;
    if (size != 0)
    {
        while (getline(file, line))
        {
            dest = line.find(',');
            s = line.substr(0, dest);
            c[sizec].userid = stoi(s);

            pos = dest + 1;
            dest = line.find(',', pos);
            s = line.substr(pos, dest - pos);
            c[sizec].firstname = s;
            pos = dest + 1;
            dest = line.find(',', pos);
            s = line.substr(pos, dest - pos);
            c[sizec].lastname = s;
            pos = dest + 1;
            dest = line.find(',', pos);
            s = line.substr(pos, dest - pos);
            c[sizec].password = s;
            pos = dest + 1;
            dest = line.find(',', pos);
            s = line.substr(pos, dest - pos);
            c[sizec].email = s;
            pos = dest + 1;
            dest = line.find('\n', pos); // how to find , when its in the next line
            s = line.substr(pos, dest - pos);
            c[sizec].phonenumber = s;
            c[size].nbreservation = 0;
            sizec++;
        }
        file.close();
        return;
    }
}
// inputs the array with the data inside the file
void fillaraycars(fstream &file, car *cr, int size)
{
    file.open("cardata.csv", ios::in);
    string line, s;
    int dest = 0, pos = 0;
    int sizec = 0;
    if (size != 0)
    {
        while (getline(file, line))
        {
            dest = line.find(',');
            s = line.substr(0, dest);
            cr[sizec].paltenumber = s;

            pos = dest + 1;
            dest = line.find(',', pos);
            s = line.substr(pos, dest - pos);
            cr[sizec].Brand = s;
            pos = dest + 1;
            dest = line.find(',', pos);
            s = line.substr(pos, dest - pos);
            cr[sizec].year = stoi(s);
            pos = dest + 1;
            dest = line.find(',', pos);
            s = line.substr(pos, dest - pos);
            cr[sizec].model = s;
            pos = dest + 1;
            dest = line.find(',', pos);
            s = line.substr(pos, dest - pos);
            cr[sizec].pricePerDaye = stod(s);
            pos = dest + 1;
            dest = line.find('\n', pos); // how to find , when its in the next line
            s = line.substr(pos, dest - pos);
            cr[sizec].color = s;
            sizec++;
        }
        file.close();
        return;
    }
}
// year check that insures tha price is onlynumbers
int yearcheck(string z)
{

    // string ---->double-->return double
    bool x;
    do
    {
        x = false;
        cout << "Enter the Year of production : ";
        getline(cin, z);
        for (int i = 0; i < z.length(); i++)
        {
            if ((z[i] >= 48 && z[i] <= 57))
            {
                x = true;
            }
            else
            {
                x = false;
                break;
            }
        }

        if (x == false)
        {
            cout << "\033[1;31mInvaled Year.\033[0m\n";
        }
    } while (x == false);
    return stoi(z);
}
// insures that model consits of only chars and  numbers only
void modelcheck(string &m)
{
    bool y;
    int x;
    do
    {
        x = 0;
        y = false;
        cout << "Enter the model of the car: ";
        getline(cin, m);
        for (int i = 0; i < m.length(); i++)
        {
            if ((m.at(i) >= 65 && m.at(i) <= 90) || (m.at(i) >= 97 && m.at(i) <= 122) || (m.at(i) >= 48 && m.at(i) <= 57) || m.at(i) == ' ')
                y = true;
            else
            {
                y = false;
                break;
            }
        }
        if (y == false)
            cout << "\033[1;31mInvalid model\033[0m\n";
    } while (y == false);
}
// checks if color is all chars with out numbers
void colorcheck(string &color)
{
    bool y;
    do
    {
        y = false;
        cout << "Enter the color of your car: ";
        getline(cin, color);
        for (int i = 0; i < color.length(); i++)
        {
            if ((color.at(i) >= 65 && color.at(i) <= 90) || (color.at(i) >= 97 && color.at(i) <= 122) || color.at(i) == ' ')
                y = true;
            else
            {
                y = false;
                break;
            }
        }
        if (y == false)
        {
            cout << "\033[1;31mThe color you entered is invlaid.\033[0m\n";
        }
    } while (y == 0);
}
// to chack the price of the car where it intakes it as string and makes it a double(not to enter loop )
double pricecheck(string z)
{
    bool x;
    int y;
    do
    {
        y = 0;
        x = false;
        cout << "Enter the daily price: ";
        getline(cin, z);
        for (int i = 0; i < z.length(); i++)
        {
            if ((z[i] >= 48 && z[i] <= 57) || z[i] == 46)
            {
                x = true;
            }
            else
            {
                x = false;
                break;
            }
            if (z[i] == 46)
            {
                y++;
            }
        }

        if (x == false || y > 1)
        {
            cout << "\033[1;31mInvaled Price.\033[0m\n";
        }
    } while (x == false || y > 1);
    return stod(z);
}
// insures thst the brand is valid having only letters and not numbers
void brandcheck(string &brand)
{
    int x;
    bool y;
    do
    {
        cout << "Enter the brand of the car(Charecter): ";
        x = 0;
        y = false;
        getline(cin, brand);
        for (int i = 0; i < brand.length(); i++)
        {
            if ((brand[i] >= 65 && brand[i] <= 90) || (brand[i] >= 97 && brand[i] <= 122))
            {
                x++;
            }
        }
        if (x == brand.length())
        {
            y = true;
        }
        if (y == false)
            cout << "\033[1;31mThe brand you entered is invalid \033[0m\n";
    } while (y == false);
}
// checks plait numbers if 6 digits and 1 char
void plaitnumbercheck(string &plaitnumber)
{
    int *p = new int[3];
    int x;
    string numbers;
    string letters;
    do
    {
        cout << "Enter the plait number(ex:123456A): ";
        x = 0;
        for (int i = 0; i < 3; i++)
        {
            *(p + i) = 0;
        }
        getline(cin, plaitnumber);
        int l = plaitnumber.length() - 1;
        plaitnumber[l] = toupper(plaitnumber[l]);
        numbers = plaitnumber.substr(0, plaitnumber.length() - 1);
        letters = plaitnumber.substr(plaitnumber.length() - 1, 1);
        if (plaitnumber.length() >= 2)
        {
            *(p + 0) = 1;
        }
        for (int i = 0; i < plaitnumber.length() - 1; i++)
        {
            if (numbers[i] >= '0' && numbers[i] <= '9')
            {
                *(p + 2) = true;
            }
            else
            {
                *(p + 2) = false;
                break;
            }
        }

        if (letters[0] >= 'A' && letters[0] <= 'Z')
        {
            *(p + 1) = 1;
        }

        if (*(p + 1) != 1 || *(p + 0) != 1 || *(p + 2) != 1)
        {
            cout << "\033[1;31m--> Invalid plaitnumber. \033[0m\n";
        }
    } while (*(p + 1) != 1 || *(p + 0) != 1 || *(p + 2) != 1);
}
// checks if the car exist in the array
bool carexist(car *cr, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (cr[i].paltenumber == cr[size].paltenumber)
        {
            return true;
        }
    }
    return false;
}
// adds cars to the array
void addcars(car *cr, int size)
{
    fstream carcsv;
    int sizee = size + 1;
    car *t = new car[sizee];
    string pr;
    for (int i = 0; i < size; i++)
    {
        t[i].paltenumber = cr[i].paltenumber;
        t[i].Brand = cr[i].Brand;
        t[i].year = cr[i].year;
        t[i].model = cr[i].model;
        t[i].pricePerDaye = cr[i].pricePerDaye;
        t[i].color = cr[i].color;
    }
    delete[] cr;
    client *i = new client;
    cr = t;
    bool z, k;
    cin.ignore();
    string ye;
    do
    {
        z = true;
        plaitnumbercheck(cr[size].paltenumber);
        if (carexist(cr, size) == true)
        {
            cout << "\033[1;31mCar plate already exists.\033[0m\n";
            z = false;
        }

    } while (z == false);
    brandcheck(cr[size].Brand);
    cr[size].year = yearcheck(ye);
    modelcheck(cr[size].model);
    cr[size].pricePerDaye = pricecheck(pr);
    colorcheck(cr[size].color);
    addcar(cr, sizee);
    admininterface();
    return;
}
// moves to administrator inter face where admins with ID equal or less than 3 can inter it
void admininterface()
{
    fstream cleint;
    int sizecc = sizereaderclients(cleint);
    client *cl = new client[sizecc];
    fillarayclients(cleint, cl, sizecc);
    array_rented(cl, sizecc);
    fstream cars;
    int size = sizereadercars(cars);
    int sizee = size + 1;
    car *cr = new car[size];
    fillaraycars(cars, cr, size);
    cout << "\033[1;33mWhat do you need to do?\n~Tap '1' to Add cars.\n~Tap '2' to Remove cars.\n~Tap '3' to Modify cars.\n~Tap '0' to exit.\033[0m " << endl;
    string x;
    do
    {
        cin >> x;
        if (x[0] == '1')
        {
            addcars(cr, size);
            return;
        }
        else if (x[0] == '2')
        {
            removecar(cr, cars, size);
            return;
        }
        else if (x[0] == '3')
        {
            modifycar(cr, size);
            return;
        }
        else if (x[0] == '0')
        {
            fill_pdf(cl, size);
            cout << "\033[1;34mThanks for your hard work.\033[0m";

            return;
        }
        else if (x.length() > 1 || x.length() == 0)
        {
            cout << "\033[1;31mInvalid coice.\033[0m\n";
        }
        else
        {
            cout << "\033[1;31mInvalid choice.\033[0m\n"
                 << endl;
        }
    } while (x[0] != '1' || x[0] != '2' || x[0] != '3' || x[0] != '0' || x.length() > 1 || x.length() == 0);
    return;
}
// displays the car data inside a file
void cardisplay(car *cr, int size)
{
    int maxPlateNumberWidth = 0;
    int maxBrandWidth = 0;
    int maxYearWidth = 0;
    int maxModelWidth = 0;
    int maxPriceWidth = 0;
    int maxColorWidth = 0;

    // Calculate the maximum width for each field and using setw() to display it
    for (int i = 0; i < size; i++)
    {
        if (cr[i].paltenumber.length() > maxPlateNumberWidth)
        {
            maxPlateNumberWidth = cr[i].paltenumber.length();
        }
        if (cr[i].Brand.length() > maxBrandWidth)
        {
            maxBrandWidth = cr[i].Brand.length();
        }
        if (to_string(cr[i].year).length() > maxYearWidth)
        {
            maxYearWidth = to_string(cr[i].year).length();
        }
        if (cr[i].model.length() > maxModelWidth)
        {
            maxModelWidth = cr[i].model.length();
        }
        if (to_string(cr[i].pricePerDaye).length() > maxPriceWidth)
        {
            maxPriceWidth = to_string(cr[i].pricePerDaye).length();
        }
        if (cr[i].color.length() > maxColorWidth)
        {
            maxColorWidth = cr[i].color.length();
        }
    }
    string pl = "Plate Number", br = "Brand", yr = "Year", md = "Model", pr = "Price", cl = "Color";
    if (pl.length() > maxPlateNumberWidth)
    {
        maxPlateNumberWidth = pl.length();
    }
    if (br.length() > maxBrandWidth)
    {
        maxBrandWidth = br.length();
    }
    if (yr.length() > maxYearWidth)
    {
        maxYearWidth = yr.length();
    }
    if (md.length() > maxModelWidth)
    {
        maxModelWidth = md.length();
    }
    if (pr.length() > maxPriceWidth)
    {
        maxPriceWidth = pr.length();
    }
    if (cl.length() > maxColorWidth)
    {
        maxColorWidth = cl.length();
    }
    // Display the data with proper spacing
    cout << left << setw(maxPlateNumberWidth + 2) << "Plate Number"
         << setw(maxBrandWidth + 2) << "Brand "
         << setw(maxYearWidth + 2) << "Year "
         << setw(maxModelWidth + 2) << "Model "
         << setw(maxPriceWidth + 2) << "Daily "
         << setw(maxColorWidth + 2) << " Color" << endl;

    for (int i = 0; i < size; i++)
    {
        cout << left << setw(maxPlateNumberWidth + 2) << cr[i].paltenumber
             << setw(maxBrandWidth + 2) << cr[i].Brand
             << setw(maxYearWidth + 2) << cr[i].year
             << setw(maxModelWidth + 2) << cr[i].model << "$"
             << setw(maxPriceWidth + 2) << cr[i].pricePerDaye
             << setw(maxColorWidth + 2) << cr[i].color << endl;
    }
}
// gets the number of reservation of cars in the file
void getreservation_number(int user, client *cl)
{
    fstream rentedcars;
    int dest = 0;
    string line, id, plate, sdate, edate;
    cl[user].nbreservation = 0;
    rentedcars.open("rentedcars.csv", ios::in);
    while (getline(rentedcars, line))
    {
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, plate, ',');
        getline(ss, sdate, ',');
        getline(ss, edate, '\n');
        if (stoi(id) == cl[user].userid)
        {
            cl[user].nbreservation = cl[user].nbreservation + 1;
        }
    }
    rentedcars.close();
}
// checks the date that the user inputs
void sdatecheck(string &date)
{
    bool x;
    do
    {
        cout << "-->";
        x = true;
        getline(cin, date);
        tm time = {};                         // in ctime lib
        istringstream datee(date);            // s stream
        datee >> get_time(&time, "%d/%m/%Y"); // function format

        if (datee.fail())
        {
            cout << "\033[1;31mInvalid format, please enter a valid one.\033[0m\n";
            x = false;
        }
        else
        {
            auto usertime = chrono::system_clock::from_time_t(mktime(&time));
            auto now = chrono::system_clock::now();
            if (usertime <=now)
            {
                cout << "\033[1;31mPlease enter a date that you can use the car in! You can't go back in time.\033[0m\n";
                x = false;
            }
        }
    } while (x == false);

    return;
}
// output the taken dates of car chosen
void displayrentalofcar(fstream &rentedcars, string platenumber)
{
    string line, start, end, id, pl;
    int x = 0, dest, pos, y = 0;
    rentedcars.open("rentedcars.csv", ios::in);

    string prevStart, prevEnd; // Variables to store the previous rental dates

    while (rentedcars)
    {
        getline(rentedcars, id, ',');
        getline(rentedcars, pl, ',');
        getline(rentedcars, start, ',');
        getline(rentedcars, end, '\n');

        if (pl == platenumber)
        {
            if (start != prevStart || end != prevEnd)
            {
                cout << "Start date: " << start << '\t' << "End date: " << end << endl;
                prevStart = start;
                prevEnd = end;
                x++;
            }
        }
    }
    if (x == 0)
    {
        cout << "You can rent this car at any time.\n";
    }
    rentedcars.close();
    return;
}
// to calculate the number of days between two dates
int claculatenumberofdays(string start, string end)
{
    tm time = {}, time2 = {};
    istringstream datee1(start);
    datee1 >> get_time(&time, "%d/%m/%Y");
    istringstream datee2(end);
    datee2 >> get_time(&time2, "%d/%m/%Y");

    auto t1 = chrono::system_clock::from_time_t(mktime(&time));
    auto t2 = chrono::system_clock::from_time_t(mktime(&time2));

    auto duration = chrono::duration_cast<chrono::hours>(t2 - t1);
    int days = duration.count() / 24; // Convert hours to days

    return days;
}
// check the end date
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
        cout << "\033[1;31mThe day you entered is before the start day.\033[0m " << endl;
        return false;
    }

    return true;
}
// checks the modifyed end date if its valid
void comparedate3_modify(string &date, string plate, string startdate, string enddate)
{
    string pl, id, line, d2;
    fstream carrent;
    string d;
    bool z, e;

    z = true;
    do
    {
        sdatecheck(date);
        carrent.open("rentedcars.csv", ios::in);
        e = false;
        z = true;

        while (carrent)
        {
            getline(carrent, id, ',');
            getline(carrent, pl, ',');
            getline(carrent, d, ',');
            getline(carrent, d2, '\n');

            tm time = {}, time2 = {}, time3 = {}, time4 = {};

            istringstream datee1(date);
            datee1 >> get_time(&time, "%d/%m/%Y");
            istringstream datee2(d);
            datee2 >> get_time(&time2, "%d/%m/%Y");
            istringstream datee3(d2);
            datee3 >> get_time(&time3, "%d/%m/%Y");
            istringstream datee4(enddate);
            datee4 >> get_time(&time4, "%d/%m/%Y");

            auto t1 = chrono::system_clock::from_time_t(mktime(&time));
            auto t2 = chrono::system_clock::from_time_t(mktime(&time2));
            auto t3 = chrono::system_clock::from_time_t(mktime(&time3));
            auto t4 = chrono::system_clock::from_time_t(mktime(&time4));
            if (pl == plate && d == startdate)
                e = true;

            if (pl == plate)
            {
                if (e == true)
                {
                    if (t1 <= t2 || t1 == t3)

                    {
                        cout << "\033[1;31mThe day you entered can't be used.\033[0m " << endl; // change it
                        z = false;
                        break;
                    }
                }
                else
                {
                    if (t1 >= t2 && t1 <= t3)
                    {
                        cout << "\033[1;31mThe day you entered conflicts with another date.\033[0m\n";
                        z = false;
                        break;
                    }
                }
            }
        }
        carrent.close();
    } while (z == false);
}
// checks the start date if its valid
void comparedate2_modify(string &date, string plate, string startdate, string enddate)
{
    string pl, id, line, d2;
    fstream carrent;
    string d;
    bool z, e;

    z = true;
    do
    {
        sdatecheck(date);
        e = false;
        z = true;
        carrent.open("rentedcars.csv", ios::in);
        while (carrent)
        {
            getline(carrent, id, ',');
            getline(carrent, pl, ',');
            getline(carrent, d, ',');
            getline(carrent, d2, '\n');

            tm time = {}, time2 = {}, time3 = {}, time4 = {}, time1 = {};

            istringstream datee1(date);
            datee1 >> get_time(&time, "%d/%m/%Y");
            istringstream datee(startdate);
            datee >> get_time(&time1, "%d/%m/%Y");
            istringstream datee2(d);
            datee2 >> get_time(&time2, "%d/%m/%Y");
            istringstream datee3(d2);
            datee3 >> get_time(&time3, "%d/%m/%Y");
            istringstream datee4(enddate);
            datee4 >> get_time(&time4, "%d/%m/%Y");

            auto t = chrono::system_clock::from_time_t(mktime(&time1));
            auto t1 = chrono::system_clock::from_time_t(mktime(&time));
            auto t2 = chrono::system_clock::from_time_t(mktime(&time2));
            auto t3 = chrono::system_clock::from_time_t(mktime(&time3));
            auto t4 = chrono::system_clock::from_time_t(mktime(&time4));
            if (pl == plate && d == startdate)
            {
                e = true;
            }
            else
                e = false;

            if (pl == plate)
            {

                if (e == true)
                {
                    if (t1 >= t4 || t1 == t)

                    {
                        cout << "\033[1;31mThe day you entered cant be used.\033[0m " << endl;
                        z = false;
                        break;
                    }
                    else
                        z = true;
                }
                else
                {
                    if (t1 >= t2 && t1 <= t3)
                    {
                        cout << "\033[1;31mThe day you entered conflicts with another date.\033[0m" << endl;
                        z = false;
                        break;
                    }
                }
            }
            if (z == false)
                break;
        }
        carrent.close();
    } while (z == false);
}
// checks the start or end dates
void comparedate2(string &date, string platenumber)
{
    string pl, id, line, d2;
    fstream carrent;
    carrent.open("rentedcars.csv", ios::in);
    string d;
    bool z;
    do
    {
        z = true;
        sdatecheck(date);
        while (carrent)
        {
            getline(carrent, id, ',');
            getline(carrent, pl, ',');
            getline(carrent, d, ',');
            getline(carrent, d2, '\n');

            if (pl == platenumber)
            {

                tm time = {}, time2 = {}, time3 = {};
                istringstream datee1(date);
                datee1 >> get_time(&time, "%d/%m/%Y");
                istringstream datee2(d);
                datee2 >> get_time(&time2, "%d/%m/%Y");
                istringstream datee3(d2);
                datee3 >> get_time(&time3, "%d/%m/%Y");

                auto t1 = chrono::system_clock::from_time_t(mktime(&time));
                auto t2 = chrono::system_clock::from_time_t(mktime(&time2));
                auto t3 = chrono::system_clock::from_time_t(mktime(&time3));
                if (t1 >= t2 && t1 <= t3)
                {
                    cout << "\033[1;31mThe day you entered conflicts with the rental of another user.\033[0m " << endl;
                    z = false;
                    break;
                }
            }
        }
    } while (z == false);
}
// compares the dates
void comparedate3(string &date, string startdate, string platenumber, bool &b)
{
    string pl, id, line, d2;
    fstream carrent;
    carrent.open("rentedcars.csv", ios::in);
    string d;
    bool z;

    z = true;
    do
    {
        sdatecheck(date);
    } while (comparedate(startdate, date) == false);
    while (carrent)
    {
        getline(carrent, id, ',');
        getline(carrent, pl, ',');
        getline(carrent, d, ',');
        getline(carrent, d2, '\n');

        if (pl == platenumber)
        {

            tm time = {}, time1 = {}, time2 = {}, time3 = {};
            istringstream datee0(startdate);
            datee0 >> get_time(&time1, "%d/%m/%Y");
            istringstream datee1(date);
            datee1 >> get_time(&time, "%d/%m/%Y");
            istringstream datee2(d);
            datee2 >> get_time(&time2, "%d/%m/%Y");
            istringstream datee3(d2);
            datee3 >> get_time(&time3, "%d/%m/%Y");

            auto t0 = chrono::system_clock::from_time_t(mktime(&time1));
            auto t1 = chrono::system_clock::from_time_t(mktime(&time));
            auto t2 = chrono::system_clock::from_time_t(mktime(&time2));
            auto t3 = chrono::system_clock::from_time_t(mktime(&time3));
            if ((t1 >= t2 && t1 <= t3) || (t0 <= t2 && t1 >= t3))
            {
                cout << "\033[1;31mThe day you entered conflicts with the rental of another user.\033[0m " << endl;
                z = false;
                break;
            }
        }
    }
    if (z == false)
    {
        b = false;
    }
}
// to fill the rented cars csv
void fill_rentedcars(fstream &rentedcars, client *cl, int user)
{

    int size = sizereaderrentedcars(rentedcars);
    if (size == 0)
    {
        rentedcars.open("rentedcars.csv", ios::out);
        rentedcars << cl[user].userid << "," << cl[user].c[size].paltenumber << "," << cl[user].c[size].d.sdate << "," << cl[user].c[size].d.edate;
    }
    else
    {
        rentedcars.open("rentedcars.csv", ios::app);
        rentedcars << endl
                   << cl[user].userid << "," << cl[user].c[cl[user].nbreservation].paltenumber << "," << cl[user].c[cl[user].nbreservation].d.sdate << "," << cl[user].c[cl[user].nbreservation].d.edate;
    }
}
// fills the car aray of the client one
void array_rented(client *cl, int size)
{
    fstream carrented;
    fstream cardata;
    for (int i = 0; i < size; i++)
    {
        getreservation_number(i, cl);
    }
    for (int i = 0; i < size; i++)
    {
        car *ca = new car[cl[i].nbreservation];
        cl[i].c = ca;
        string ID, plait, start, end;
        int id, s = 0;
        carrented.open("rentedcars.csv", ios::in);
        while (getline(carrented, ID, ','))
        {

            id = stoi(ID);

            getline(carrented, plait, ',');
            getline(carrented, start, ',');
            getline(carrented, end, '\n');
            if (id == cl[i].userid)
            {
                cl[i].c[s].paltenumber = plait;
                cl[i].c[s].d.sdate = start;
                cl[i].c[s].d.edate = end;
                s++;
            }
        }
        carrented.close();
        string pl, price, year, brand, color, model;
        cardata.open("cardata.csv", ios::in);
        while (cardata)
        {
            getline(cardata, pl, ',');
            getline(cardata, brand, ',');
            getline(cardata, year, ',');
            getline(cardata, model, ',');
            getline(cardata, price, ',');
            getline(cardata, color, '\n');
            for (int j = 0; j < cl[i].nbreservation; j++)
            {
                if (pl == cl[i].c[j].paltenumber)
                {
                    cl[i].c[j].Brand = brand;
                    cl[i].c[j].year = stoi(year);
                    cl[i].c[j].model = model;
                    price = price.substr(0, '$');
                    cl[i].c[j].pricePerDaye = stod(price);
                    cl[i].c[j].color = color;
                }
            }
        }
        cardata.close();
    }
}
// searches for the car by its plate number
int plaitnumbersearch(car *cr, int size, string plaitnumber)
{
    int pos = -1;
    do
    {
        for (int i = 0; i < size; i++)
        {
            if (cr[i].paltenumber == plaitnumber)
            {
                pos = i;
                break;
            }
        }
        if (pos == -1)
        {
            cout << "\033[1;31mThe car is not available.\033[0m\n";
            plaitnumbercheck(plaitnumber);
        }
    } while (pos == -1);
    return pos;
}
// to add cars to array and then to the fill by sending it to fill array
void adda_a_carto_array(client *cl, int user, int sizek)
{
    array_rented(cl, sizek);
    bool b;
    int sizee = cl[user].nbreservation + 1;
    car *t = new car[sizee];
    for (int i = 0; i < cl[user].nbreservation; i++)
    {
        t[i].paltenumber = cl[user].c[i].paltenumber;
        t[i].Brand = cl[user].c[i].Brand;
        t[i].year = cl[user].c[i].year;
        t[i].pricePerDaye = cl[user].c[i].pricePerDaye;
        t[i].color = cl[user].c[i].color;
        t[i].d.sdate = cl[user].c[i].d.sdate;
        t[i].d.edate = cl[user].c[i].d.edate;
    }
    delete[] cl[user].c;
    cl[user].c = t;
    fstream cardata, rentedcars;
    string plaitnumber, startdate, enddate;
    double pricetopay;
    tm date = {};
    int pos, nbrdays;
    int size = sizereadercars(cardata);
    car *cr = new car[size]; // to store all the preasent cars in an array
    fillaraycars(cardata, cr, size);
    cardisplay(cr, size);
    cin.ignore();
    plaitnumbercheck(plaitnumber);
    pos = plaitnumbersearch(cr, size, plaitnumber);
    cl[user].c[cl[user].nbreservation].paltenumber = plaitnumber;
    displayrentalofcar(rentedcars, cl[user].c[cl[user].nbreservation].paltenumber);
    do
    {
        b = true;
        cout << "Enter the date that you want to rent the car in (dd/mm/yyyy): ";
        comparedate2(startdate, cl[user].c[cl[user].nbreservation].paltenumber);
        cl[user].c[cl[user].nbreservation].d.sdate = startdate;
        cout << "Enter the date that you want to end the rent in (dd/dm/yyyy): ";
        comparedate3(enddate, startdate, cl[user].c[cl[user].nbreservation].paltenumber, b);

    } while (b == false);

    cl[user].c[cl[user].nbreservation].d.edate = enddate;
    nbrdays = claculatenumberofdays(startdate, enddate);
    pricetopay = cr[pos].pricePerDaye * nbrdays;
    cout << "You should pay :" << pricetopay << "$" << endl
         << "\033[34mThanks for using our service, you can head to our company to take your car at \033[0m" << cl[user].c[cl[user].nbreservation].d.sdate << endl;
    fill_rentedcars(rentedcars, cl, user);
    return;
}
// searches for the first user rental
void firstone(client *cl, int user, int size, int &x)
{
    for (int i = 0; i < size; i++)
    {
        if (cl[i].nbreservation)
        {
            x = i;
            break;
        }
    }
}
// delets the rented car of the user
void fillcarrentel_modify_delete(client *cl, fstream &rentedcars, int user, int size)
{
    int s = 0;
    firstone(cl, user, size, s);
    rentedcars.open("rentedcars.csv", ios::trunc);
    rentedcars.close();
    rentedcars.open("rentedcars.csv", ios::out);
    rentedcars << cl[s].userid << "," << cl[s].c[0].paltenumber << "," << cl[s].c[0].d.sdate << "," << cl[s].c[0].d.edate;
    rentedcars.close();
    if (cl[s].nbreservation > 1)
    {
        rentedcars.open("rentedcars.csv", ios::app);
        for (int i = 1; i < cl[s].nbreservation; i++)
        {
            rentedcars << endl
                       << cl[s].userid << "," << cl[s].c[i].paltenumber << "," << cl[s].c[i].d.sdate << "," << cl[s].c[i].d.edate;
        }
        rentedcars.close();
    }
    rentedcars.open("rentedcars.csv", ios::app);
    for (int i = s + 1; i < size; i++)
    {

        for (int j = 0; j < cl[i].nbreservation; j++)
        {
            rentedcars << endl
                       << cl[i].userid << "," << cl[i].c[j].paltenumber << "," << cl[i].c[j].d.sdate << "," << cl[i].c[j].d.edate;
        }
    }
    rentedcars.close();
}
// to delete th rental
void removerentedcar(client *cl, int user, int size)
{
    fstream rentedcars;
    array_rented(cl, size);
    if (cl[user].nbreservation > 0)
    {
        displayuserrental(cl, size, user);
        string platenumber;
        int n;
        int sizee = cl[user].nbreservation - 1;
        car *t = new car[sizee];

        do
        {
            n = -1;
            int b = 0;
            string sdatee;
            cin.ignore();
            plaitnumbercheck(platenumber);
            for (int i = 0; i < cl[user].nbreservation; i++)
            {
                if (cl[user].c[i].paltenumber == platenumber)
                {
                    n = i;
                    b++;
                }
                if (b > 1)
                {
                    break;
                }
            }
            if (b > 1)
            {
                cout << "Enter the start date of the car, it seems that you rent this car several times.\n";
                do
                {
                    n = -1;
                    cout << "please enter the start date of the one you want to remove:";
                    getline(cin, sdatee);
                    for (int z = 0; z < cl[user].nbreservation; z++)
                    {
                        if ((cl[user].c[z].d.sdate == sdatee) && (platenumber == cl[user].c[z].paltenumber))
                        {
                            n = z;
                            break;
                        }
                    }
                    if (n == -1)
                    {
                        cout << "\033[1;31mPlease enter a valid date.\033[0m\n";
                    }

                } while (n == -1);
            }

            if (n == -1)
            {
                cout << "\033[1;31mThis plait number dose not exist\033[0m";
            }
            else
            {
                for (int i = 0; i < n; i++)
                {
                    t[i].paltenumber = cl[user].c[i].paltenumber;
                    t[i].Brand = cl[user].c[i].Brand;
                    t[i].year = cl[user].c[i].year;
                    t[i].pricePerDaye = cl[user].c[i].pricePerDaye;
                    t[i].color = cl[user].c[i].color;
                    t[i].d.sdate = cl[user].c[i].d.sdate;
                    t[i].d.edate = cl[user].c[i].d.edate;
                }
                for (int i = n; i < sizee; i++)
                {
                    t[i].paltenumber = cl[user].c[i + 1].paltenumber;
                    t[i].Brand = cl[user].c[i + 1].Brand;
                    t[i].year = cl[user].c[i + 1].year;
                    t[i].pricePerDaye = cl[user].c[i + 1].pricePerDaye;
                    t[i].color = cl[user].c[i + 1].color;
                    t[i].d.sdate = cl[user].c[i + 1].d.sdate;
                    t[i].d.edate = cl[user].c[i + 1].d.edate;
                }
                delete[] cl[user].c;
                cl[user].c = t;
            }
        } while (n == -1);
        cl[user].nbreservation = cl[user].nbreservation - 1;
        cout << "The car has been removed\n";
        fillcarrentel_modify_delete(cl, rentedcars, user, size);
    }
    else
        cout << "You have no cars to rent\n";

    return;
}
// modifys the car selected
void modify_rentedcar(client *cl, int user, int size)
{
    fstream rentedcars;
    fstream cardata;
    int q;
    sizereadercars(rentedcars);
    array_rented(cl, size);
    if (cl[user].nbreservation > 0)
    {

        bool z;
        int n, d;
        string pl;
        displayuserrental(cl, size, user);
        cout << "Enter the plait number of the car you want to modify.\n";
        do
        {

            n = -1;
            int b = 0;
            string sdatee;
            cin.ignore();
            plaitnumbercheck(pl);
            for (int i = 0; i < cl[user].nbreservation; i++)
            {
                if (cl[user].c[i].paltenumber == pl)
                {
                    n = i;
                    b++;
                }
                if (b > 1)
                {
                    break;
                }
            }
            cout << "hello\n";
            if (b > 1)
            {
                cout << "Enter the start date of the car, it seems that you rent this car several times.\n";
                do
                {
                    n = -1;
                    cout << "please enter the start date of the one you want to remove:";
                    getline(cin, sdatee);
                    for (int z = 0; z < cl[user].nbreservation; z++)
                    {
                        if ((cl[user].c[z].d.sdate == sdatee) && (cl[user].c[z].paltenumber == pl))
                        {
                            n = z;
                            break;
                        }
                    }
                    if (n == -1)
                    {
                        cout << "\033[1;31m--> Please enter a valid date.\033[0m\n";
                    }
                } while (n == -1);
            }
            cout << "\033[1;33mWhat do you want to change?\033[0m\n";
            cout << "\033[1;33m~Tap '1'to change start date\033[0m\n";
            cout << "\033[1;33m~Tap '2'to change end date\033[0m\n";
            string choice;
            do
            {
                getline(cin, choice);
                if (choice[0] != '1' && choice[0] != '2' || choice.length() > 1)
                {
                    cout << "\033[1;31mPlease enter a valid choice.\033[0m\n";
                }
            } while (choice[0] != '1' && choice[0] != '2' || choice.length() > 1);
            string date;
            if (choice[0] == '1')
            {
                comparedate2_modify(date, pl, cl[user].c[n].d.sdate, cl[user].c[n].d.edate);
                cl[user].c[n].d.sdate = date;
            }
            else
            {

                comparedate3_modify(date, pl, cl[user].c[n].d.sdate, cl[user].c[n].d.edate);

                cl[user].c[n].d.edate = date;
            }
        } while (n == -1);

        fillcarrentel_modify_delete(cl, rentedcars, user, size);
    }
    else
        cout << "You have no car to modify\n";
    return;
}
// tacks input from user
void userinterface(client *c, int user, int size)
{
    int choice;
    fstream car;
    do
    {
        cout << "\033[1;33mWhat do you need to do?\n~Tap '1' to Rent a car.\n~Tap '2' to Remove any rented car.\n"
             << "~Tap '3' to Modify your rental date.\nTap '0' to exit.\033[0m" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            adda_a_carto_array(c, user, size);
            break;
        case 2:
            removerentedcar(c, user, size);

            break;
        case 3:
            modify_rentedcar(c, user, size);
            break;
        case 0:
        {
            fill_pdf(c, size);
            cout << "<> Thanks for using our service, have a good day." << endl;
            return;
        }
        default:
            cout << "\033[1;31m-->Invalid choice.\033[0m" << endl;
            break;
        }
    } while (choice != 1 || choice != 2 || choice != 3 || choice != 0);
}
// add car to the user
void displayuserrental(client *cl, int size, int user)
{
    array_rented(cl, size);
    fstream rentedcars;
    fstream cars;
    string id, pl, sdate, edate;
    int carsize;
    if (cl[user].nbreservation != 0)
    {
        cout << "--> Your rented cars are:" << endl;

        int maxPlateNumberWidth = 0;
        int startDateWidth = 0;
        int endDateWidth = 0;

        for (int i = 0; i < cl[user].nbreservation; i++)
        {
            if (cl[user].c[i].paltenumber.length() > maxPlateNumberWidth)
            {
                maxPlateNumberWidth = cl[user].c[i].paltenumber.length();
            }
            if (cl[user].c[i].d.sdate.length() > startDateWidth)
            {
                startDateWidth = cl[user].c[i].d.sdate.length();
            }
            if (cl[user].c[i].d.edate.length() > endDateWidth)
            {
                endDateWidth = cl[user].c[i].d.edate.length();
            }
        }

        string plHeader = "Plate Number", sdHeader = "Start Date", edHeader = "End Date";
        if (plHeader.length() > maxPlateNumberWidth)
        {
            maxPlateNumberWidth = plHeader.length();
        }
        if (sdHeader.length() > startDateWidth)
        {
            startDateWidth = sdHeader.length();
        }
        if (edHeader.length() > endDateWidth)
        {
            endDateWidth = edHeader.length();
        }

        cout << left << setw(maxPlateNumberWidth + 2) << plHeader
             << setw(startDateWidth + 2) << sdHeader
             << setw(endDateWidth + 2) << edHeader << endl;

        for (int i = 0; i < cl[user].nbreservation; i++)
        {
            cout << left << setw(maxPlateNumberWidth + 2) << cl[user].c[i].paltenumber
                 << setw(startDateWidth + 2) << cl[user].c[i].d.sdate
                 << setw(endDateWidth + 2) << cl[user].c[i].d.edate << endl;
        }
    }
    else
    {
        cout << " You have no rented cars." << endl;
    }

    // needs fixationnn
    cars.close();
}
// searchs array to see if ID and pass are same to login
void login(client *c, int size)
{
    string password, idd;
    bool *p = new bool[1];
    int x;
    do
    {
        *(p + 1) = false;
        cout << "ID : ";
        getline(cin, idd);
        cout << "Password : ";
        getline(cin, password);
        for (int i = 0; i < size + 1; i++)
        {
            // if (to_string(c[i].userid) == idd && c[i].password == sha1(password))
            {
                *(p + 1) = true;
                x = c[i].userid;
                break;
            }
        }
        if (*(p + 1) == false)
        {

            cout << "\033[1;31m Wrong email or password, please enter a correct one\n";
            cout << "\033[0m"; // Reset text color to default
        }
        else
            cout << "\033[1;34m Welcome back.\033[0m\n";

    } while (*(p + 1) == false);
    if (x <= 3)
    {
        admininterface();
        return;
    }
    else
    {

        userinterface(c, x - 1, size);
        return;
    }
}
// lets user choose between login and sighn up
void choice(client *c, int size)
{
    char x;
    cout << "\033[1;33m~Press 1 to login .\n~Press 2 to sighn up.\n~Press 0 to exit.\033[0m " << endl;
    do
    {
        cin >> x;
        if (x == '2')
        {
            cin.ignore();
            sighnup(c, size);
        }
        else if (x == '1')
        {
            cin.ignore();
            login(c, size);
        }
        else if (x == '0')
        {

            cout << "Thanks for using our service.";
            fill_pdf(c, size);
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
//
void beginapp()
{

    for (int i = 0; i < 148; i++)
        cout << "-";
    cout << endl;
    cout << "\033[1mCompaney Name :Green companey \033[0m\n\n\n\n";
    for (int i = 0; i < 148; i++)
        cout << "-";
    cout << endl;
    cout << "\033[1mMission statment :\033[0m\n  Car Rentals, our mission is to provide convenient, reliable, and affordable transportation solutions to our customers.\n We strive to exceed expectations by offering a diverse fleet of well-maintained vehicles, exceptional customer service.\n Our commitment to sustainability drives us to continually explore eco-friendly initiatives ,reducing our environmental footprint while contributing positively to the communities we serve.\n With a focus on safety, integrity, and continuous improvement, we aim to be the .\npreferred choice for all your rental needs, delivering unparalleled value and peace of mind with every journey.\n\n\n";
    for (int i = 0; i < 148; i++)
        cout << "-";
    cout << endl;
    cout << "\033[1mTarget audience :\033[0m \n\n";
    cout << "1.Shop owners : It will make their work very easy and they will record all the information on the application without any problems with delivery, time, or anything else.\n";
    cout << "2.Travelers: This includes tourists, business travelers, and individuals going on vacations who need temporary transportation solutions.\n";
    cout << "3.Local Residents: People who need a car for short-term use, such as running errands, attending events, or making occasional trips where owning a car may not be practical.\n";
    cout << "4.Businesses: Companies that require rental vehicles for business purposes, such as transporting employees, clients, or equipment.\n";
    cout << "5.Students: College students or young professionals who may not own a car and need a temporary vehicle for commuting, moving, or recreational purposes.\n";
    for (int i = 0; i < 148; i++)
        cout << "-";
    cout << endl;
}
// sorts the price per day
void sort(car *ca, int s, double *t)
{
    for (int i = 0; i < s; i++)
    {
        t[i] = ca[i].pricePerDaye;
    }
    double temp;
    for (int i = 0; i < s; i++)
    {
        for (int i = 0; i < s - 1; i++)
        {
            if (t[i] > t[i + 1])
            {
                temp = t[i];
                t[i] = t[i + 1];
                t[i + 1] = temp;
            }
        }
    }
    return;
}

// to fill the pdf
// void fill_pdf(client *cl, int sizee)
// {
//     fstream clients;
//     int size = sizereaderclients(clients);
//     fillarayclients(clients, cl, size);
//     array_rented(cl, size);
//     fstream cars;
//     int s = sizereadercars(cars);
//     car *ca = new car[s];
//     fillaraycars(cars, ca, s);
//     vector<string> inputs;
//     double *t = new double[s];
//     sort(ca, s, t);
//     int l = size + 1, weadth2 = 10, userweadth = 0;
//     int plaitnbweadth = 0, firstnameweadth = 0, lastnameweadth = 0, sdateweadth = 0, edateweadth = 0;
//     while (l != 0)
//     {
//         l /= 10;
//         userweadth++;
//     }
//     for (int i = 0; i < size; i++)
//     {

//         if (cl[i].firstname.length() > firstnameweadth)
//         {
//             firstnameweadth = cl[i].firstname.length();
//         }
//         if (cl[i].lastname.length() > lastnameweadth)
//         {
//             lastnameweadth = cl[i].lastname.length();
//         }
//         for (int j = 0; j < cl[i].nbreservation; j++)
//         {
//             if (cl[i].c[j].paltenumber.length() > plaitnbweadth)
//             {
//                 plaitnbweadth = cl[i].c[j].paltenumber.length();
//             }
//             if (cl[i].c[j].d.sdate.length() > sdateweadth)
//             {
//                 sdateweadth = cl[i].c[j].d.sdate.length();
//             }
//             if (cl[i].c[j].d.edate.length() > edateweadth)
//             {
//                 edateweadth = cl[i].c[j].d.edate.length();
//             }
//         }
//     }

//     for (int q = 0; q < s; q++)
//     {
//         for (int i = 0; i < size; i++)
//         {
//             for (int j = 0; j < cl[i].nbreservation; j++)
//             {
//                 if (t[q] == cl[i].c[j].pricePerDaye)
//                 {
//                     stringstream ss;
//                     ss << setw(userweadth + 3) << left << to_string(cl[i].userid);
//                     ss << setw(userweadth + 12) << left << cl[i].firstname;
//                     ss << setw(firstnameweadth + 12) << left << cl[i].lastname;
//                     ss << setw(lastnameweadth + 12) << left << cl[i].c[j].paltenumber;
//                     ss << setw(plaitnbweadth + 12) << left << cl[i].c[j].d.sdate;
//                     ss << setw(sdateweadth + 12) << left << cl[i].c[j].d.edate;
//                     ss << setw(edateweadth + 12) << left << cl[i].phonenumber;
//                     inputs.push_back(ss.str());
//                 }
//             }
//         }
//     }
//     HPDF_Doc pdf = HPDF_New(NULL, NULL);
//     if (!pdf)
//     {
//         cout << "Error: Unable to create PDF document." << endl;
//         return;
//     }

//     HPDF_Page page = HPDF_AddPage(pdf);
//     HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
//     if (!page)
//     {
//         cout << "Error: Unable to add a page to the PDF." << endl;
//         HPDF_Free(pdf);
//         return;
//     }

//     HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", "WinAnsiEncoding");
//     if (!font)
//     {
//         cout << "Error: Unable to load font." << endl;
//         HPDF_Free(pdf);
//         return;
//     }
//     HPDF_Page_SetFontAndSize(page, font, 12);

//     float y_position = HPDF_Page_GetHeight(page) - 50;
//     for (const auto &input : inputs)
//     {
//         HPDF_Page_BeginText(page);
//         HPDF_Page_MoveTextPos(page, 50, y_position);
//         HPDF_Page_ShowText(page, input.c_str());
//         HPDF_Page_EndText(page);
//         y_position -= 20;
//     }

//     if (HPDF_SaveToFile(pdf, "rentedcars.pdf") != HPDF_OK)
//     {
//         cout << "Error: Unable to save the PDF." << endl;
//         HPDF_Free(pdf);
//         return;
//     }

//     HPDF_Free(pdf);
//     cout << "Please enter your feedback.\n-->";
//     string j;
//     cin.ignore();
//     getline(cin, j);

//     cout << "PDF created successfully." << endl;

//     return;
// }

// checks if the rented cars are after the end date then delete the car
void checker()
{
    fstream rentedcars;
    tm time = {};
    rentedcars.open("rentedcars.csv", ios::in);
    int x = 0, y = 0;
    string line, pl, sdate, edate, ID;
    while (getline(rentedcars, ID, ',') && getline(rentedcars, pl, ',') &&
           getline(rentedcars, sdate, ',') && getline(rentedcars, edate, '\n'))
    {
        istringstream datee(edate);
        datee >> get_time(&time, "%d/%m/%Y");
        auto t1 = chrono::system_clock::from_time_t(mktime(&time));
        auto now = chrono::system_clock::now();
        if (t1 >= now)
        {
            x++;
        }
        y++;
    }
    rentedcars.close();

    if (x > 0 && x != y)
    {
        client *ca = new client[x];
        car *cq = new car[x];
        rentedcars.open("rentedcars.csv", ios::in);
        int id;
        int i = 0;
        while (getline(rentedcars, ID, ',') && getline(rentedcars, pl, ',') &&
               getline(rentedcars, sdate, ',') && getline(rentedcars, edate, '\n'))
        {
            istringstream datee(edate);
            datee >> get_time(&time, "%d/%m/%Y");
            auto t1 = chrono::system_clock::from_time_t(mktime(&time));
            auto now = chrono::system_clock::now();
            if (t1 >= now)
            {
                id = stoi(ID);
                ca[i].userid = stoi(ID);
                cq[i].paltenumber = pl;
                cq[i].d.sdate = sdate;
                cq[i].d.edate = edate;
                i++;
            }
        }
        rentedcars.close();
        for (int i = 0; i < x; i++)
        {
            if (i == 0)
            {
                rentedcars.open("rentedcars.csv", ios::out);
                rentedcars << ca[i].userid << "," << cq[i].paltenumber << "," << cq[i].d.sdate << "," << cq[i].d.edate;
            }
            else
            {
                rentedcars << endl
                           << ca[i].userid << "," << cq[i].paltenumber << "," << cq[i].d.sdate << "," << cq[i].d.edate;
            }
        }
    }

    return;
}

int main()
{
    fstream clients;
    int size = sizereaderclients(clients);
    client *cl = new client[size];
    checker();
    beginapp();
    fillarayclients(clients, cl, size);
    array_rented(cl, size);
    choice(cl, size);
    return 0;
}