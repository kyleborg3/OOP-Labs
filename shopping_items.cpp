#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//Parent class

class Item {
protected:
    string name;
    double price;

public:
    //Default
    Item(const string& n = "", double p = 0.0) : name(n), price(p) {}

    virtual ~Item() {} //Destructor to clean up resources

    virtual void display() const { //Display basic info which could be overridden
        cout << "Name: " << name << " | Price: " << price;
    }

    virtual void persist(ofstream& ofs) const = 0; //Each subclass will write itself to a file
    virtual string getType() const = 0; //Returns a string identifier for the subclass
};

//Subclasses

class Grocery : public Item {
    string expirationDate;
public:
    Grocery(const string& n = "", double p = 0.0, const string& exp = "")
        : Item(n, p), expirationDate(exp) {}

    void display() const override {
        cout << "[Grocery] "; Item::display();
        cout << " | Expiration: " << expirationDate << endl;
    }

    void persist(ofstream& ofs) const override {
        ofs << "GROCERY|" << name << "|" << price << "|" << expirationDate << endl;
    }

    string getType() const override { return "GROCERY"; }

    static Grocery* createFromLine(const string& line) { //Reads a line and returns a new Grocery object
        stringstream ss(line);
        string type, n, priceStr, exp;
        getline(ss, type, '|'); getline(ss, n, '|');
        getline(ss, priceStr, '|'); getline(ss, exp, '|');
        return new Grocery(n, stod(priceStr), exp);
    }
};

//Similarly for the rest of the subclasses:
class Electronics : public Item {
    int warrantyYears;
public:
    Electronics(const string& n = "", double p = 0.0, int w = 0)
        : Item(n, p), warrantyYears(w) {}

    void display() const override {
        cout << "[Electronics] "; Item::display();
        cout << " | Warranty: " << warrantyYears << " years" << endl;
    }

    void persist(ofstream& ofs) const override {
        ofs << "ELECTRONICS|" << name << "|" << price << "|" << warrantyYears << endl;
    }

    string getType() const override { return "ELECTRONICS"; }

    static Electronics* createFromLine(const string& line) {
        stringstream ss(line);
        string type, n, priceStr, wStr;
        getline(ss, type, '|'); getline(ss, n, '|');
        getline(ss, priceStr, '|'); getline(ss, wStr, '|');
        return new Electronics(n, stod(priceStr), stoi(wStr));
    }
};

class Clothing : public Item {
    string size;
public:
    Clothing(const string& n = "", double p = 0.0, const string& s = "")
        : Item(n, p), size(s) {}

    void display() const override {
        cout << "[Clothing] "; Item::display();
        cout << " | Size: " << size << endl;
    }

    void persist(ofstream& ofs) const override {
        ofs << "CLOTHING|" << name << "|" << price << "|" << size << endl;
    }

    string getType() const override { return "CLOTHING"; }

    static Clothing* createFromLine(const string& line) {
        stringstream ss(line);
        string type, n, priceStr, size;
        getline(ss, type, '|'); getline(ss, n, '|');
        getline(ss, priceStr, '|'); getline(ss, size, '|');
        return new Clothing(n, stod(priceStr), size);
    }
};

class Book : public Item {
    string author;
public:
    Book(const string& n = "", double p = 0.0, const string& a = "")
        : Item(n, p), author(a) {}

    void display() const override {
        cout << "[Book] "; Item::display();
        cout << " | Author: " << author << endl;
    }

    void persist(ofstream& ofs) const override {
        ofs << "BOOK|" << name << "|" << price << "|" << author << endl;
    }

    string getType() const override { return "BOOK"; }

    static Book* createFromLine(const string& line) {
        stringstream ss(line);
        string type, n, priceStr, author;
        getline(ss, type, '|'); getline(ss, n, '|');
        getline(ss, priceStr, '|'); getline(ss, author, '|');
        return new Book(n, stod(priceStr), author);
    }
};

class Toy : public Item {
    int recommendedAge;
public:
    Toy(const string& n = "", double p = 0.0, int age = 0)
        : Item(n, p), recommendedAge(age) {}

    void display() const override {
        cout << "[Toy] "; Item::display();
        cout << " | Recommended Age: " << recommendedAge << "+" << endl;
    }

    void persist(ofstream& ofs) const override {
        ofs << "TOY|" << name << "|" << price << "|" << recommendedAge << endl;
    }

    string getType() const override { return "TOY"; }

    static Toy* createFromLine(const string& line) {
        stringstream ss(line);
        string type, n, priceStr, ageStr;
        getline(ss, type, '|'); getline(ss, n, '|');
        getline(ss, priceStr, '|'); getline(ss, ageStr, '|');
        return new Toy(n, stod(priceStr), stoi(ageStr));
    }
};

//Restore function
vector<Item*> restore(const string& filename) { //Reads the file line by line, reads the type identifier, calls createfromLine() and returns the items
    vector<Item*> items;
    ifstream ifs(filename);
    if (!ifs) return items;

    string line;
    while (getline(ifs, line)) {
        stringstream ss(line);
        string type;
        getline(ss, type, '|');

        if (type == "GROCERY") items.push_back(Grocery::createFromLine(line));
        else if (type == "ELECTRONICS") items.push_back(Electronics::createFromLine(line));
        else if (type == "CLOTHING") items.push_back(Clothing::createFromLine(line));
        else if (type == "BOOK") items.push_back(Book::createFromLine(line));
        else if (type == "TOY") items.push_back(Toy::createFromLine(line));
    }
    return items;
}

//Main program
int main() {
    vector<Item*> items = restore("items.txt"); //Loads any previously saved items

    cout << "Restored " << items.size() << " item(s) from file.\n";

    while (true) {
        cout << "\n1. Add Grocery\n2. Add Electronics\n3. Add Clothing\n4. Add Book\n5. Add Toy\n6. Display All\n7. Save & Exit\nChoice: "; //Menu
        int choice;
        cin >> choice;
        if (choice == 7) break;

        string name;
        double price;

        switch (choice) { //Menu choices
            case 1: {
                string exp;
                cout << "Enter name, price, expiration: ";
                cin >> name >> price >> exp;
                items.push_back(new Grocery(name, price, exp));
                break;
            }
            case 2: {
                int w;
                cout << "Enter name, price, warranty years: ";
                cin >> name >> price >> w;
                items.push_back(new Electronics(name, price, w));
                break;
            }
            case 3: {
                string size;
                cout << "Enter name, price, size: ";
                cin >> name >> price >> size;
                items.push_back(new Clothing(name, price, size));
                break;
            }
            case 4: {
                string author;
                cout << "Enter name, price, author: ";
                cin >> name >> price >> author;
                items.push_back(new Book(name, price, author));
                break;
            }
            case 5: {
                int age;
                cout << "Enter name, price, recommended age: ";
                cin >> name >> price >> age;
                items.push_back(new Toy(name, price, age));
                break;
            }
            case 6:
                for (auto* item : items) item->display(); //Displays all items
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }

    ofstream ofs("items.txt");
    for (auto* item : items)
        item->persist(ofs);
    ofs.close();

    for (auto* item : items)
        delete item;

    cout << "Saved all items to items.txt\n";

    //Saves all items and deletes all dynamically allocated memory to prevent memory leaks
    
    return 0;
}
