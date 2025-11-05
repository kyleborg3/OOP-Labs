#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <queue>
#include <string>
using namespace std;

//Parent class
class Item {
protected: //Common properties
    string name;
    double price;
public:
    Item(const string& n = "", double p = 0.0) : name(n), price(p) {}
    virtual ~Item() = default;

    virtual void display() const = 0;
    virtual string getType() const = 0;

    virtual void persistBinary(ofstream& ofs) const = 0;
    static unique_ptr<Item> restoreBinary(ifstream& ifs);

    bool operator<(const Item& other) const { return price < other.price; }

    friend ostream& operator<<(ostream& os, const Item& item);
    friend istream& operator>>(istream& is, unique_ptr<Item>& item);
};

//Sub classes
class Grocery : public Item {
    string expiry;
public:
    Grocery(const string& n = "", double p = 0.0, const string& e = "") : Item(n, p), expiry(e) {}
    void display() const override {
        cout << "Grocery - " << name << " (€" << price << ") Exp: " << expiry << endl; //Prints item info
    }
    string getType() const override { return "Grocery"; } //Returns type as string
    void persistBinary(ofstream& ofs) const override { //Save object fields to binary
        size_t len = name.size(); ofs.write((char*)&len, sizeof(len)); ofs.write(name.c_str(), len);
        ofs.write((char*)&price, sizeof(price));
        len = expiry.size(); ofs.write((char*)&len, sizeof(len)); ofs.write(expiry.c_str(), len);
    }
    static unique_ptr<Item> restoreBinary(ifstream& ifs) { //Restores object fields from binary
        size_t len; string n, e; double p;
        ifs.read((char*)&len, sizeof(len)); n.resize(len); ifs.read(&n[0], len);
        ifs.read((char*)&p, sizeof(p));
        ifs.read((char*)&len, sizeof(len)); e.resize(len); ifs.read(&e[0], len);
        return make_unique<Grocery>(n, p, e);
    }
};

class Electronics : public Item {
    int warranty;
public:
    Electronics(const string& n = "", double p = 0.0, int w = 0) : Item(n, p), warranty(w) {}
    void display() const override {
        cout << "Electronics - " << name << " (€" << price << ") Warranty: " << warranty << "y" << endl;
    }
    string getType() const override { return "Electronics"; }
    void persistBinary(ofstream& ofs) const override {
        size_t len = name.size(); ofs.write((char*)&len, sizeof(len)); ofs.write(name.c_str(), len);
        ofs.write((char*)&price, sizeof(price));
        ofs.write((char*)&warranty, sizeof(warranty));
    }
    static unique_ptr<Item> restoreBinary(ifstream& ifs) {
        size_t len; string n; double p; int w;
        ifs.read((char*)&len, sizeof(len)); n.resize(len); ifs.read(&n[0], len);
        ifs.read((char*)&p, sizeof(p));
        ifs.read((char*)&w, sizeof(w));
        return make_unique<Electronics>(n, p, w);
    }
};

class Clothing : public Item {
    string size;
public:
    Clothing(const string& n = "", double p = 0.0, const string& s = "") : Item(n, p), size(s) {}
    void display() const override {
        cout << "Clothing - " << name << " (€" << price << ") Size: " << size << endl;
    }
    string getType() const override { return "Clothing"; }
    void persistBinary(ofstream& ofs) const override {
        size_t len = name.size(); ofs.write((char*)&len, sizeof(len)); ofs.write(name.c_str(), len);
        ofs.write((char*)&price, sizeof(price));
        len = size.size(); ofs.write((char*)&len, sizeof(len)); ofs.write(size.c_str(), len);
    }
    static unique_ptr<Item> restoreBinary(ifstream& ifs) {
        size_t len; string n, s; double p;
        ifs.read((char*)&len, sizeof(len)); n.resize(len); ifs.read(&n[0], len);
        ifs.read((char*)&p, sizeof(p));
        ifs.read((char*)&len, sizeof(len)); s.resize(len); ifs.read(&s[0], len);
        return make_unique<Clothing>(n, p, s);
    }
};

class Book : public Item {
    string author;
public:
    Book(const string& n = "", double p = 0.0, const string& a = "") : Item(n, p), author(a) {}
    void display() const override {
        cout << "Book - " << name << " (€" << price << ") Author: " << author << endl;
    }
    string getType() const override { return "Book"; }
    void persistBinary(ofstream& ofs) const override {
        size_t len = name.size(); ofs.write((char*)&len, sizeof(len)); ofs.write(name.c_str(), len);
        ofs.write((char*)&price, sizeof(price));
        len = author.size(); ofs.write((char*)&len, sizeof(len)); ofs.write(author.c_str(), len);
    }
    static unique_ptr<Item> restoreBinary(ifstream& ifs) {
        size_t len; string n, a; double p;
        ifs.read((char*)&len, sizeof(len)); n.resize(len); ifs.read(&n[0], len);
        ifs.read((char*)&p, sizeof(p));
        ifs.read((char*)&len, sizeof(len)); a.resize(len); ifs.read(&a[0], len);
        return make_unique<Book>(n, p, a);
    }
};

class Toy : public Item {
    int recommendedAge;
public:
    Toy(const string& n = "", double p = 0.0, int age = 0) : Item(n, p), recommendedAge(age) {}
    void display() const override {
        cout << "Toy - " << name << " (€" << price << ") Recommended Age: " << recommendedAge << "+" << endl;
    }
    string getType() const override { return "Toy"; }
    void persistBinary(ofstream& ofs) const override {
        size_t len = name.size(); ofs.write((char*)&len, sizeof(len)); ofs.write(name.c_str(), len);
        ofs.write((char*)&price, sizeof(price));
        ofs.write((char*)&recommendedAge, sizeof(recommendedAge));
    }
    static unique_ptr<Item> restoreBinary(ifstream& ifs) {
        size_t len; string n; double p; int age;
        ifs.read((char*)&len, sizeof(len)); n.resize(len); ifs.read(&n[0], len);
        ifs.read((char*)&p, sizeof(p));
        ifs.read((char*)&age, sizeof(age));
        return make_unique<Toy>(n, p, age);
    }
};

//Factory function
unique_ptr<Item> Item::restoreBinary(ifstream& ifs) { //Reads item type from file
    string type; size_t len;
    ifs.read((char*)&len, sizeof(len));
    if (!ifs) return nullptr;
    type.resize(len); ifs.read(&type[0], len);

    if (type == "Grocery") return Grocery::restoreBinary(ifs);
    if (type == "Electronics") return Electronics::restoreBinary(ifs);
    if (type == "Clothing") return Clothing::restoreBinary(ifs);
    if (type == "Book") return Book::restoreBinary(ifs);
    if (type == "Toy") return Toy::restoreBinary(ifs);
    return nullptr;
}


//Container class
class Container { 
    shared_ptr<vector<unique_ptr<Item>>> items; //Holds all items
public:
    Container() : items(make_shared<vector<unique_ptr<Item>>>()) {}
    //Adds, prints, saved, loads all items and gives access for the priority queue
    void add(unique_ptr<Item> i) { items->push_back(move(i)); }
    void showAll() const { for (const auto& i : *items) i->display(); }
    void saveBinary(const string& file) {
        ofstream ofs(file, ios::binary);
        for (const auto& i : *items) {
            size_t len = i->getType().size();
            ofs.write((char*)&len, sizeof(len));
            ofs.write(i->getType().c_str(), len);
            i->persistBinary(ofs);
        }
    }
    void loadBinary(const string& file) {
        ifstream ifs(file, ios::binary);
        items->clear();
        while (ifs.peek() != EOF) {
            auto i = Item::restoreBinary(ifs);
            if (i) items->push_back(move(i));
        }
    }
    vector<unique_ptr<Item>>& getItems() { return *items; }
};

//Main class
int main() {
    Container c;

    //Load items if exist
    ifstream test("items.bin", ios::binary);
    if (test) {
        test.close();
        c.loadBinary("items.bin");
    }

    while (true) {
        cout << "\nMenu:\n"; //Menu display
        cout << "1. Add Grocery\n2. Add Electronics\n3. Add Clothing\n4. Add Book\n5. Add Toy\n6. Show All\n7. Save & Exit\nChoice: ";
        int choice;
        cin >> choice;

        if (choice == 7) break;

        string name;
        double price;

        switch (choice) {
            case 1: {
                string expiry;
                cout << "Enter name, price, expiry: ";
                cin >> name >> price >> expiry;
                c.add(make_unique<Grocery>(name, price, expiry));
                break;
            }
            case 2: {
                int warranty;
                cout << "Enter name, price, warranty years: ";
                cin >> name >> price >> warranty;
                c.add(make_unique<Electronics>(name, price, warranty));
                break;
            }
            case 3: {
                string size;
                cout << "Enter name, price, size: ";
                cin >> name >> price >> size;
                c.add(make_unique<Clothing>(name, price, size));
                break;
            }
            case 4: {
                string author;
                cout << "Enter name, price, author: ";
                cin >> name >> price >> author;
                c.add(make_unique<Book>(name, price, author));
                break;
            }
            case 5: {
                int age;
                cout << "Enter name, price, recommended age: ";
                cin >> name >> price >> age;
                c.add(make_unique<Toy>(name, price, age));
                break;
            }
            case 6: {
                // Show all items
                cout << "\n=== All Items ===\n";
                c.showAll();

                // Show priority queue (descending price)
                cout << "\n=== Priority Queue (by descending price) ===\n";
                auto cmp = [](Item* a, Item* b) { return *a < *b; };
                priority_queue<Item*, vector<Item*>, decltype(cmp)> pq(cmp);

                for (auto& item : c.getItems()) pq.push(item.get());

                while (!pq.empty()) {
                    pq.top()->display();
                    pq.pop();
                }
                break;
            }
            default:
                cout << "Invalid choice!\n";
        }
    }

    //Save everything back to file
    c.saveBinary("items.bin");
    cout << "\nItems saved successfully!\n";

    return 0;
}