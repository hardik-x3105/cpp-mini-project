#include <iostream>
#include <fstream> //file handling
#include <vector> //vectors
#include <string> //strings

using namespace std;

// Base class for- food, fuel and equipments
class CargoItem
{
protected:
    string id, name, priority;
    double weight, length, width, height;

public:
    CargoItem(string id, string name, double weight, string priority, double length, double width, double height)
        : id(id), name(name), weight(weight), priority(priority), length(length), width(width), height(height) {}

    virtual void display() = 0;
    virtual string serialize() = 0;

    double getWeight()
    {
        return weight;
    }

    string getPriority()
    {
        return priority;
    }

    string getID()
    {
        return id;
    }

    double getVolume()
    {
        return length * width * height;
    }

    virtual ~CargoItem() {}
};

//derived class (type one- fuel)
class Fuel : public CargoItem
{
public:
    Fuel(string id, string name, double weight, string priority, double length, double width, double height)
        : CargoItem(id, name, weight, priority, length, width, height) {}

    void display()
    {
        cout << "[Fuel] " << id << " - " << name;
        cout << " (" << weight << "kg, " << getVolume() << " m^3, " << priority << " priority)\n";
    }

    string serialize()
    {
        return "Fuel," + id + "," + name + "," + to_string(weight) + "," +
               priority + "," + to_string(length) + "," + to_string(width) + "," + to_string(height);
    }
};

//derived class (type two- food)
class Food : public CargoItem
{
public:
    Food(string id, string name, double weight, string priority, double length, double width, double height)
        : CargoItem(id, name, weight, priority, length, width, height) {}

    void display()
    {
        cout << "[Food] " << id << " - " << name;
        cout << " (" << weight << "kg, " << getVolume() << " m^3, " << priority << " priority)\n";
    }

    string serialize()
    {
        return "Food," + id + "," + name + "," + to_string(weight) + "," +
               priority + "," + to_string(length) + "," +
               to_string(width) + "," + to_string(height);
    }
};

//derived class (type three- equipment)
class Equipment : public CargoItem
{
public:
    Equipment(string id, string name, double weight, string priority, double length, double width, double height)
        : CargoItem(id, name, weight, priority, length, width, height) {}

    void display()
    {
        cout << "[Equipment] " << id << " - " << name;
        cout << " (" << weight << "kg, " << getVolume() << " m^3, " << priority << " priority)\n";
    }

    string serialize()
    {
        return "Equipment," + id + "," + name + "," + to_string(weight) + "," +
               priority + "," + to_string(length) + "," +
               to_string(width) + "," + to_string(height);
    }
};

//managing all data via vecotrs
vector<CargoItem*> cargoList;
const double MAX_CAPACITY = 2000.0;
const double MAX_VOLUME = 1000.0;

void addCargo()
{
    string type, id, name, priority;
    double weight, length, width, height;

    cout << "\nEnter cargo type (Fuel/Food/Equipment): ";
    cin >> type;
    cin.ignore();

    cout << "Enter ID: ";
    getline(cin, id);
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Weight (kg): ";
    cin >> weight;
    cin.ignore();

    cout << "Enter Priority (High/Medium/Low): ";
    getline(cin, priority);

    cout << "Enter Dimensions (L W H in meters): ";
    cin >> length >> width >> height;

    double totalWeight = 0, totalVolume = 0;
    for (int i = 0; i < cargoList.size(); i++) {
        totalWeight += cargoList[i]->getWeight();
        totalVolume += cargoList[i]->getVolume();
    }

    double newVolume = length * width * height;

    if (totalWeight + weight > MAX_CAPACITY) {
        cout << "Error: Exceeding cargo weight capacity!\n";
        return;
    }

    if (totalVolume + newVolume > MAX_VOLUME) {
        cout << "Error: Exceeding cargo space volume!\n";
        return;
    }

    if (type == "Fuel")
        cargoList.push_back(new Fuel(id, name, weight, priority, length, width, height));
    else if (type == "Food")
        cargoList.push_back(new Food(id, name, weight, priority, length, width, height));
    else if (type == "Equipment")
        cargoList.push_back(new Equipment(id, name, weight, priority, length, width, height));
    else {
        cout << "Invalid cargo type!\n";
        return;
    }

    cout << "Cargo added successfully!\n";
}

//Display all Cargo items
void displayAll()
{
    if (cargoList.empty())
    {
        cout << "No cargo loaded.\n";
        return;
    }
    for (int i = 0; i < cargoList.size(); i++)
        cargoList[i]->display();
}

//file handling for cargo
void saveToFile(string filename)
{
    ofstream out(filename);
    for (int i = 0; i < cargoList.size(); i++)
        out << cargoList[i]->serialize() << endl;
    cout << "Data saved to " << filename << "\n";
}

void loadFromFile(string filename)
{
    ifstream in(filename);
    cargoList.clear();
    string type, id, name, priority;
    double weight, length, width, height;

    while (getline(in, type, ',')) {
        getline(in, id, ',');
        getline(in, name, ',');
        in >> weight;
        in.ignore();
        getline(in, priority, ',');
        in >> length;
        in.ignore();
        in >> width;
        in.ignore();
        in >> height;
        in.ignore();

        if (type == "Fuel")
            cargoList.push_back(new Fuel(id, name, weight, priority, length, width, height));

        else if (type == "Food")
            cargoList.push_back(new Food(id, name, weight, priority, length, width, height));

        else if (type == "Equipment")
            cargoList.push_back(new Equipment(id, name, weight, priority, length, width, height));
    }

    cout << "Data loaded from " << filename << "\n";
}

void searchByPriority(string value) {
    bool found = false;
    for (int i = 0; i < cargoList.size(); i++) {
        if (cargoList[i]->getPriority() == value) {
            cargoList[i]->display();
            found = true;
        }
    }
    if (!found)
        cout << "No cargo with priority: " << value << "\n";
}

//Main
int main() {
    int choice;
    while (true) {
        cout << "\nSpace Station Cargo Menu:  \n";
        cout << "1. Add Cargo\n2. Display All Cargo\n3. Save to File\n4. Load from File\n5. Search by Priority\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addCargo();
                break;
            case 2:
                displayAll();
                break;
            case 3:
                saveToFile("cargo_data.txt");
                break;
            case 4:
                loadFromFile("cargo_data.txt");
                break;
            case 5:
            {
                string p;
                cout << "Enter priority to search: ";
                cin >> p;
                searchByPriority(p);
                break;
            }
            case 6:
                for (int i = 0; i < cargoList.size(); i++)
                    delete cargoList[i];
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
}
