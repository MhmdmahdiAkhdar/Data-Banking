#include <iostream>
#include<string>
#include<cstring>
#include<cmath>
#include<fstream>

using namespace std;

struct sword {
	char data[30];
	sword* next;
};
struct dword {
	char data[30];
	sword* syn, * ant;
	dword* next, * prev;
};
struct dictionary {
	dword* head, * tail;
};

void addsynant(dictionary*);
dictionary* readfile(dictionary*);
void writefile(dictionary*);
void swapdblylinks(dword*&, dword*, dword*&, dword*&);
void sortdic(dictionary*&);
void createword(dictionary*&, dword*);
void printsynsingly(dword*, sword*, char[]);
void printsyn(sword*);
void printant(sword*);
void sta(string&, char[]);
bool dwordexist(dword*, char[]);
bool swordexist(sword*, char[]);
int wordpos(dictionary*, char[]);
bool startsamelet(char[], char[],int size);
bool sameletters(char[], char[]);
void deletehead(dictionary*&);
void deletetail(dictionary*&);
void deletethisword(dword*);
dword* deletewordslet(dictionary*&);
void searchstartlet(dictionary*);
void printantspecial(dword*);
void displaysynant(dictionary*, char[]);
void deleteallwords(dictionary*);
void displaysynant(dictionary*, char[]);
void search(dictionary*);
void mainfun(dictionary*&);
void split(dictionary*&);
void deletesingly(sword*);
void deletedict(dictionary*&);
void printlist(dword* head);
#pragma warning(disable : 4996)

void sta(string& word, char c[]) {
	strcpy(c, word.c_str());
}
bool dwordexist(dword* head, char c[])   //function to confirm the existance of the word in the dictionary
{
	dword* tmp = head;
	int i = 0;
	while (c[i] != '\0')
	{
		c[i] = tolower(c[i]);                       //convert all the characters to lower case
		i++;
	}
	while (tmp != NULL)
	{
		if (strcmp(tmp->data, c) == 0)
			return true;                 //returns true if the word exist
	}
	return false;                        // returns false if the word doesn't exist
}
bool swordexist(sword* head, char c[])   //function to confirm the existance of an sword 
{
	if (head == NULL)
		return false;
	sword* tmp = head;
	int i = 0;
	while (c[i] != '\0')
	{
		c[i] = tolower(c[i]);            //convert to lower case for comparison
		i++;
	}
	while (tmp != NULL)
	{
		if (strcmp(tmp->data, c) == 0)
		{
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}
int wordpos(dictionary* d, char c[])            //function to search all the dictionary for a word and returns its position
{
	int i = 1;
	dword* cur = d->head;
	while (cur != NULL)
	{
		if (dwordexist(cur, c) == true)
		{
			return i;
		}
		cur = cur->next;
		i++;
	}
	int j = 1;
	dword* tmp = d->head;
	while (tmp != NULL)
	{
		if (swordexist(tmp->syn, c) == true || swordexist(tmp->ant, c) == true)
		{
			return j;
		}
		tmp = tmp->next;
		j++;
	}
	return 0;
}

void printdictionary(dictionary* d)
{
	dword* cur = d->head;
	while (cur != NULL)
	{
		cout << "Word: " << cur->data << endl;
		cout << "synonyms: ";
		printsyn(cur->syn);
		cout << "antonyms: ";
		printant(cur->ant);
		cout << endl;
		cur = cur->next;
	}
}


void printsyn(sword* head)
{
	if (head == NULL)
	{
		cout << "The list is empty, no synonyms found" << endl;
	}
	sword* cur = head;
	while (cur != NULL)
	{
		cout << cur->data << " ";
		cur = cur->next;
	}
	cout << endl;
}
void printant(sword* head)
{
	if (head == NULL)
	{
		cout << "There are no antonyms found for " << head << endl;
	}
	sword* cur = head;
	while (cur != NULL)
	{
		cout << cur->data << " ";
		cur = cur->next;
	}
	cout << endl;
}
void subaddsynant(dword* d)
{
	char x;
	sword* s = d->syn;
	sword* a = d->ant;
	string w;
	sword* tmps = new sword;
	cout << "Okay, found the word! Please choose whether you need to add a synonym or an antonym(s or a): ";
	cin >> x;
	while (x != 's' && x != 'a' && x != 'A' && x != 'S')
	{
		cout << "The letter you entered is not correct. Re-enter: ";
		cin >> x;
	}
	if (x == 's' || x == 'S')
	{
		cout << "Please enter the word: ";
		getline(cin, w);
		char* c = new char[w.length() + 1];
		sta(w, c);
		for (int i = 0; c[i] != '\0'; i++)
		{
			tmps->data[i] = c[i];
		}
		while (s->next != NULL)
		{
			s = s->next;
		}
		s->next = tmps;
		tmps->next = NULL;
		cout << "Successfully added!" << endl;
	}
	else if (x == 'a' || x == 'A')
	{
		cout << "Please enter the word: ";
		getline(cin, w);
		char* c = new char[w.length() + 1];
		sta(w, c);
		for (int i = 0; c[i] != '\0'; i++)
		{
			tmps->data[i] = c[i];
		}
		while (a->next != NULL)
		{
			a = a->next;
		}
		a->next = tmps;
		tmps->next = NULL;
		cout << "Successfully added!" << endl;
	}
	cin.ignore();

}

void addsynant(dictionary* d)
{
	cin.ignore();
	dictionary* a = d;
	dword* tmp = d->head;
	int i;
	char x;
	string  w;
	cout << "Please enter the word you want to add the synonym or antonym for :";
	cin.ignore();
	getline(cin, w);
	char* c = new char[w.length() + 1];
	sta(w, c);
	int b = 0;
	while (c[b] != '\0')
	{
		c[b] = tolower(c[b]);
		b++;
	}
	i = wordpos(a, c);
	if (i == 0)
	{
		cout << "Error! The word is not found in the dictionary.";
	}
	else
		for (int j = 1; j <= i; j++)
		{
			if (j == i)
			{
				if (strcmp(tmp->data, c) == 0)
				{
					subaddsynant(tmp);
				}
			}
			tmp = tmp->next;
		}
	cin.ignore();

}

void createword(dictionary*& d, dword* head)
{
	dword* neword = new dword;
	string str;
	int syn = 0, ant = 0;
	int x;
	cout << "How many words you want to add: ";
	cin >> x;
	cin.ignore();
	while (x > 0)
	{
		cout << "Please enter the word you want to add:";
		getline(cin, str);
		sta(str, neword->data);
		while (dwordexist(head, neword->data) != 0 && neword->data[0] != ' ')
		{
			cout << "Error! This word already exist please enter a new word: ";
			getline(cin, str);
			sta(str, neword->data);
			cin.ignore();
		}
		cout << "How many synonyms do you want to add: ";
		cin >> syn;
		while (syn < 0)
		{
			cout << "Error! Please re-enter the number of synonyms you want to add: ";
			cin >> syn;
		}
		sword* synonym = new sword;
		sword* curs = synonym;
		cin.ignore();
		for (int i = 0; i < syn; i++)
		{
			cout << "Enter the " << i + 1 << " synonym: ";
			getline(cin, str);
			sta(str, curs->data);
			if (i != syn - 1) {
				curs->next = new sword;
				curs = curs->next;
			}
		}
		curs->next = NULL;
		if (syn == 0)
			synonym = NULL;
		neword->syn = synonym;
		cout << "How many antonyms do you want to add: ";
		cin >> ant;
		while (ant < 0)
		{
			cout << "Error! Please re-enter the number of antonyms you want to add: ";
			cin >> ant;
		}
		sword* antonym = new sword;
		sword* cura = antonym;
		cin.ignore();
		for (int i = 0; i < ant; i++)
		{
			cout << "Enter the " << i + 1 << " antonym: ";
			getline(cin, str);
			sta(str, cura->data);
			if (i != ant - 1) {
				cura->next = new sword;
				cura = cura->next;
			}
		}
		cura->next = NULL;
		if (ant == 0)
			antonym = NULL;
		neword->ant = antonym;

		x--;
	}
	sortdic(d);

	return;
}

sword* insertattail(sword* head, char c[])
{
	sword* n = new sword;
	int i = 0;
	for (; c[i] != '\0'; i++)
		n->data[i] = c[i];
	n->data[i] = '\0';
	if (head == NULL)							//for the exception of an empty list we insert at head
	{
		head = n;
		head->next = NULL;
		return head;
	}
	sword* cur = head;
	while (cur->next != NULL)					//loop till the end of the list
	{
		cur = cur->next;
	}
	cur->next = n;								//place the new node n at the tail
	n->next = NULL;								//set the next of the tail to NULL
	return head;
}

int cntwrdsinfile()
{
	int i = 0;
	ifstream dict("dictionary.txt");			//enters the file
	string word;
	while (dict)								//while file hasn't ended
	{
		getline(dict, word, '\n');				//takes data line by line
		i++;
	}
	return i - 1;								//returns i-1 (gets rid of the empty line at the end of the file)
}

dictionary* rdfile(dictionary* d)
{
	dword* cur = new dword;						//create a current node to be used to move through the list
	dword* prv = NULL;							//create a previous pointer that will be used to fill the previous of the doubly linked list
	d->head = new dword;						//creates the head pointer;
	d->tail = new dword;
	cur = d->head;
	cur->prev = NULL;						//set head->previous to null
	string word;
	ifstream dict("dictionary.txt");
	if (cntwrdsinfile() <= 0)
	{
		cout << "Error reading file or empty file";
		return NULL;
	}
	for (int fl = 0; fl < cntwrdsinfile(); fl++)//Start reading file untill there is no data left to read
	{
		getline(dict, word, '\n');				//reads the first line of the file to the string word
		int i = 0;
		bool b = 0;
		if (word != "")							//checks if the line is empty
		{
			while (word.length() > i)			//while the string still has characters
			{
				for (; word[i] != ':' && b == 0 && word[i] != '#'; i++)
					cur->data[i] = word[i];		//writes the first word in the line to the doubly linked list  
				if (b == 0)						//to ensure that the doubly linked list data is only changed once per line
				{
					cur->data[i] = '\0';		//sets the end of the array of characters
					b = 1;
					cur->syn = NULL;		//initialize the synonym singly linked list
					cur->ant = NULL;		//initialize the antonym singly linked list
				}
				if (word[i] == ':')				//if the synonym word started
				{
					i++;						//don't consider the : character
					sword* s = new sword;		//sets the location in the memory were the synonym list will be saved
					int j = 0;					//counter of the synonym word
					for (; word[i] != ':' && word[i] != '#' && word[i] != '\n'; i++, j++)
					{
						s->data[j] = word[i];	//fills the words tp the synonym list
					}
					s->data[j] = '\0';			//set the end of the array of characters for each synonym
					cur->syn = insertattail(cur->syn, s->data);//inserts the synonym in the list
				}
				if (word[i] == '#')				//same for antonyms
				{
					i++;
					sword* a = new sword;
					int j = 0;
					for (; i < word.length() && word[i] != '#' && word[i] != '\n'; i++, j++)
					{
						a->data[j] = word[i];
					}
					a->data[j] = '\0';
					cur->ant = insertattail(cur->ant, a->data);
				}
			}
			prv = cur;							//set previous to current
			cur = new dword;					//create the next node
			prv->next = cur;					//set the next to the next node
			cur->prev = prv;				//set the previous of the next node	
		}
	}
	d->tail = prv;								//set the tail of the coubly linked list to the last node
	if (prv != NULL)
		d->tail->next = NULL;
	dict.close();
	return d;
}


void wfile(dictionary* d)
{
	dword* cur = d->head;						//set the current to start from head		
	ofstream dict("dictionary.txt");
	while (cur != NULL)							//loop untill the end of the list
	{
		dict << cur->data;						//write the word to file
		sword* scur = cur->syn;				//pointer to its related synonyms
		while (scur != NULL)
		{
			dict << ":" << scur->data;			//write the synonyms
			scur = scur->next;
		}
		sword* acur = cur->ant;				//pointer to the related antonyms
		while (acur != NULL)
		{
			dict << "#" << acur->data;			//write the antonyms
			acur = acur->next;
		}
		dict << '\n';							//end the line when the antonyms are finished
		cur = cur->next;						//next word
	}
	dict.close();
}


void swapdblylinks(dword*& a, dword* b, dword*& head, dword*& tail)
{
	dword* tmpb = b->next, * tmpa = a->prev;
	if (a->prev != NULL)
		tmpa->next = b;
	else
		head = b;								//a is the head of the list so we change b to the head on swap
	if (b->next != NULL)
		tmpb->prev = a;
	else
	{
		tail = a;
		a->next = NULL;							//b is the tail of the list so we change a to the tail on swap
	}
	b->next = a;
	a->next = tmpb;
	b->prev = tmpa;
	a->prev = b;
}
/*void swapdblynodes(dword* a, dword* b)
{
	if (a->next == NULL && b->prev == NULL)
	{
		dword* tmpb = b->next->prev;
		a->next = b->next;
		a->next->prev = a->prev->next;
		a->prev->next = tmpb;
		b->prev = a->prev;
		b->next = NULL;
		a->prev = NULL;
	}
	else if (a->next != NULL && b->prev == NULL)
	{
		dword* tmp1 = b->next->prev;
		dword* tmp2 = a->next;
		a->next = b->next;
		a->next->prev = a->prev->next;
		a->prev->next = tmp1;
		b->prev = a->prev;
		b->next->prev = tmp2;
		b->next = b->prev->next->next;
		a->prev = NULL;
	}
	else if (a->prev == NULL && b->next == NULL)
	{
		dword* tmp = a->next->prev;
		b->next = a->next;
		b->next->prev = b->prev->next;
		b->prev->next = tmp;
		a->prev = b->prev;
		a->next = NULL;
		b->prev = NULL;
	}
	else if (b->next != NULL && a->prev == NULL)
	{
		dword* tmp1 = a->next->prev;
		dword* tmp2 = b->next;
		b->next = a->next;
		b->next->prev = b->prev->next;
		b->prev->next = tmp1;
		a->prev = b->prev;
		a->next->prev = tmp2;
		a->next = a->prev->next->next;
		b->prev = NULL;
	}
	else {
		dword* tmp1 = a->next->prev;
		dword* tmp2 = b->next->prev;
		a->next = b->next;
		a->next->prev = tmp1;
		b->next = a->prev->next->next;
		b->next->prev = tmp2;
		dword* tmp3 = b->prev;
		b->prev = a->prev;
		a->prev = tmp3;
	}
}*/
void sortdic(dictionary*& d)           //function for sorting the words in alphabetical order 
{
	dword* cur = d->head;
	while (cur != NULL)
	{
		dword* cur1 = d->head;
		while (cur1->next != NULL)
		{
			if (strcmp(cur1->data, cur1->next->data) > 0)
				swapdblylinks(cur1, cur1->next, d->head, d->tail);
			cur1 = cur1->next;
			if (cur1 == NULL)
				break;
		}
		if (cur == NULL)
			return;
		cur = cur->next;
	}
}
bool startsamelet(char c1[], char c2[], int size)               //function to check whether the array c1 start with the same letters found in c2
{
	for (int k = 0; k < size; k++)
	{
		if (c1[k] != c2[k])
			return false;
	}
	return true;
}
bool sameletters(char c1[], char c2[])                   //function to check whether the array of characters c1 have the same letters of c2
{
	int l2 = 0, l1 = 0;
	for (int i = 0; c2[i] != '\0'; i++)
	{
		l2++;
	}
	for (int j = 0; c1[j] != '\0'; j++)
	{
		l1++;
	}
	int count = 0;
	for (int a = 0; a <= l2 && c2[a] != '\0'; a++)
	{
		int x = c2[a];
		for (int k = 0; k <= l1 && c1[k] != '\0'; k++)
		{
			if (c1[k] == c2[a])
			{
				count++;
				break;
			}
		}
	}
	if (count >= l2)
		return true;
	else
		return false;
}
void deletehead(dictionary*& d)                     //function that deletes the head
{
	dword* cur = d->head;
	sword* curs = d->head->syn;
	sword* tmp;
	while (curs != NULL)
	{
		tmp = curs->next;
		delete curs;
		curs = tmp;
	}
	sword* cura = d->head->ant;
	while (cura != NULL)
	{
		tmp = cura->next;
		delete cura;
		cura = tmp;
	}
	dword* x;
	x = d->head->next;
	delete d->head;
	d->head = x;
	d->head->prev = NULL;
}
void deletetail(dictionary*& d)                       //function that deletes the tail
{
	dword* cur = d->tail;
	sword* curs = d->tail->syn;
	sword* tmp;
	while (curs != NULL)
	{
		tmp = curs->next;
		delete curs;
		curs = tmp;
	}
	sword* cura = d->head->ant;
	while (cura != NULL)
	{
		tmp = cura->next;
		delete cura;
		cura = tmp;
	}
	dword* x;
	x = d->tail->prev;
	delete d->tail;
	d->tail = x;
	d->tail->next = NULL;
}

void deletethisword(dword* d)                                      //function that deletes a specific words with its syn and ant
{
	sword* curs = d->syn;
	sword* tmp;
	while (curs != NULL)
	{
		tmp = curs->next;
		delete curs;
		curs = tmp;
	}
	sword* cura = d->ant;
	while (cura != NULL)
	{
		tmp = cura->next;
		delete cura;
		cura = tmp;
	}
	d->next->prev = d->prev;
	d->prev->next = d->next;
	delete d;
}

dword* deletewordslet(dictionary*& d)                         //function that deletes all words having these letters
{
	dword* cur = d->head;
	int count1 = 0, count2 = 0;
	int size;
	cout << "How many letter you want to specify: ";
	cin >> size;
	cout << "endl";
	char* c = new char[size];
	cout << "Please enter the letters , character by character: ";
	for (int i = 0; i < size; i++)
	{
		cin >> c[i];
	}
	while (cur != NULL)
	{
		if (sameletters(cur->data, c) == true)
		{
			if (cur == d->head)
			{
				deletehead(d);
			}
			else if (cur == d->tail)
			{
				deletetail(d);
			}
			else
			{
				deletethisword(cur);
			}
		}
		else
			count1++;

		cur = cur->next;
		count2++;
	}
	if (count1 != count2)
	{
		return d->head;
	}
	else if (count1 == count2)
	{
		cout << "Error! No words in the dictionary contain these letters " << endl;
	}

	cin.ignore();
	return d->head;
}
void searchstartlet(dictionary* d)                               //function that searches the dictionary for a word starting with certain letters
{
	dword* cur = d->head;
	char l[30];
	cout << "Please enter the letters that the word starts with to search: ";
	cin.getline(l, 30);
	while (cur != NULL)
	{
		if (startsamelet(cur->data, l,30) == true)
		{
			cout << cur->data << endl;
		}
		cur = cur->next;
	}
	cin.ignore();
	return;
}
void printsynsingly(dword* head, sword* head1, char c[])
{
	sword* cur = head1;
	dword* tmp = head;
	sword* curs = tmp->syn;
	if (head == NULL)
	{
		cout << "Error! No synonyms found " << endl;
		return;
	}
	else
	{
		while (cur != NULL)
		{
			if (cur->data == c)
			{
				cout << tmp->data << " ";
				while (curs != NULL)
				{
					if (curs->data == cur->data)
					{
						curs = curs->next;
					}
					cout << curs->data << " ";

					curs = curs->next;
				}
			}
			cur = cur->next;
		}

	}
}
void printantsingly(dword* head, sword* head1, char c[])
{
	sword* cur = head1;
	dword* tmp = head;
	sword* cura = tmp->ant;
	if (head == NULL)
	{
		cout << "No antonyms found!" << endl;
		return;
	}
	else
	{
		while (cur != NULL)
		{
			if (cur->data == c)
			{
				while (cura != NULL)
				{
					if (cura->data == cur->data)
					{
						cura = cura->next;
					}
					cout << cura->data << " ";

					cura = cura->next;
				}
			}
			cur = cur->next;
		}
	}
}
void printantspecial(dword* head)
{
	if (head == NULL)
	{
		cout << "The list is empty, no synonyms found" << endl;
	}
	dword* cur = head;
	sword* currs = new sword;
	currs = cur->syn;
	cout << cur->data << " - ";
	while (currs != NULL)
	{
		cout << currs->data << " ";
		currs = currs->next;
	}
	cout << endl;
}
void displaysynant(dictionary* d, char c[])                                             //function that displays the synonyms and antonyms for a specific word
{
	dword* cur = d->head;
	while (cur != NULL)
	{
		if (dwordexist(cur, c) == true)
		{
			cout << "Synonyms: "; printsyn(cur->syn);
			cout << "Antonyms: "; printant(cur->ant);
			return;
		}
		else if (swordexist(cur->syn, c) == true)
		{
			cout << "Synonyms: "; printsynsingly(cur, cur->syn, c);
			cout << "Antonyms: "; printant(cur->ant);
			return;
		}
		else if (swordexist(cur->ant, c) == true)
		{
			cout << "Synonyms: "; printantsingly(cur, cur->ant, c);
			cout << "Antonyms: "; printantspecial(cur);
			return;
		}
		cur = cur->next;
	}
	cout << "The word is not found!" << endl;
}
void deleteallwords(dictionary* d)                                     //function that deletes all words starting with specific letters
{
	string w;
	dword* cur = d->head;
	cout << "Please enter the letters that the word start with: ";
	getline(cin, w);
	char* c = new char[w.length()];
	sta(w, c);
	while (cur != NULL)
	{
		if (startsamelet(cur->data, c,w.length()) == true && cur == d->head)
		{
			deletehead(d);
		}
		else if (startsamelet(cur->data, c,w.length()) == true && cur == d->tail)
		{
			deletetail(d);
		}
		else if (startsamelet(cur->data, c,w.length()) == true)
		{
			deletethisword(cur);
		}
		cur = cur->next;
	}
	cin.ignore();
	cout << "Done! All the words starting with '" << w << "' are deleted" << endl;
}
void search(dictionary* d)
{
	cin.ignore();
	dword* tmp = d->head;
	string w;
	cout << "Please enter the word you want to search for: ";
	getline(cin, w);
	char* c = new char[w.length() + 1];
	sta(w, c);
	while (tmp != NULL)
	{
		if (strcmp(tmp->data, c) == 0)
		{
			displaysynant(d, c);
			return;
		}
		tmp = tmp->next;
	}
	cout << "Word not found!" << endl;
}
dword* insert(dword*& newH, dword* oldH) {
	if (newH == NULL) {
		newH = oldH;
		newH->next = NULL;
		newH->prev = NULL;
		return newH;
	}
	else {
		dword* cur = newH;
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = oldH;
		oldH->prev = cur;
		oldH->next = NULL;
		return newH;
	}
}
void printlist(dword* head) {
	dword* tmp = head;
	while (tmp != NULL) {
		cout << tmp->data << endl;
		tmp = tmp->next;
	}
}
void deletesingly(sword* head)
{
	sword* cur = head;
	sword* tmp;
	while (cur != NULL)
	{
		tmp = cur->next;
		delete cur;
		cur = tmp;
	}
}
void deletedict(dictionary*& d)
{
	dword* cur = d->head;
	while (cur != NULL)
	{
		deletesingly(cur->syn);
		deletesingly(cur->ant);
		dword* tmp = cur->next;
		delete cur;
		cur = tmp;
	}
	d->head = NULL;
}
void split(dictionary*& d)
{
	string w;
	dword* cur = d->head;
	dword* tmp1 = new dword;
	dword* tmp2 = new dword;
	int size;
	int count = 0;
	cout << "According to how many letters you want to split the dictionary: ";
	cin >> size;
	char* c = new char[size];
	cin.ignore();
	cout << "Please enter the letters you want to split the dictionary according to: ";
	for (int i = 0; i < size; i++)
	{
		cin >> c[i];
	}
	cin.ignore();		
	while (cur != NULL)
	{
		if (startsamelet(cur->data, c,size) == true)
		{
			tmp1 = insert(tmp1, cur);
		}
		else
		{
			tmp2 = insert(tmp2, cur);
		}
		cur = cur->next;
	}
	deletedict(d);
	cout << "LIST 1:" << endl; printlist(tmp1);
	cout << "LIST 2:" << endl; printlist(tmp2);

}
void mainfun(dictionary*& d)
{
	int x;
	char b;
	char c[30];
	string w;
	cout << "Hello! Welcome to the data structures dictionary" << endl;
	cin.ignore();
	do {
		cout << "Please tell me your choice: " << endl;
		cout << "1. Display the dictionary" << endl;
		cout << "2. Search for a specific word" << endl;
		cout << "3. Search for a word starting with specific letters" << endl;
		cout << "4. Display the synonyms and antonyms for a certain word" << endl;
		cout << "5. Add some new words" << endl;
		cout << "6. Add a synonym or an antonym" << endl;
		cout << "7. Delete all words starting with certain letters" << endl;
		cout << "8. Delete all words containing specific letters" << endl;
		cout << "9. Splits the dictionary according to certain letters" << endl;
		cout << "10. Exit dictionary" << endl;
		cin.ignore();
		do {
			cout << "Your option: ";
			cin >> x;
		} while (x < 1 || x>10);
		if (x == 1)
		{
			printdictionary(d);
		}
		else if (x == 2)
		{
			search(d);
		}
		else if (x == 3)
		{
			searchstartlet(d);
		}
		else if (x == 4)
		{
			cin.ignore();
			cout << "Please enter the word you want to search for: ";
			cin.getline(c, 30);
			displaysynant(d, c);
		}
		else if (x == 5)
		{
			cin.ignore();
			createword(d, d->head);

		}
		else if (x == 6)
		{
			addsynant(d);
		}
		else if (x == 7)
		{
			deleteallwords(d);
		}
		else if (x == 8)
		{
			deletewordslet(d);
		}
		else if (x == 9)
		{
			split(d);
		}
		else if (x == 10)
		{
			cout << "You left the dictionary! Have a nice day!" << endl;
			return;
		}
		cout << "Do you want to perform other actions (yes or no (y/n) ) ?" << endl;
		cin >> b;
		cin.ignore();

	} while (b == 'y' || b == 'Y');

}



int main()
{
	dictionary* d = new dictionary;
	d = rdfile(d);
	cout<<"hello";
	if (d == NULL)
		return 0;
	sortdic(d);
	mainfun(d);
	wfile(d);
}
