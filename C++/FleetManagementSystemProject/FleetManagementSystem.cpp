// Author: Maxime Pol Marcet.
// I implement a CLI-based fleet management system in C++ to simulate commercial vehicle operations.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// I define the structure and behavior of a vehicle.
class Vehicle {
public:
    string plate;
    string brand;
    string model;
    int year;
    string fuel;
    float loadCapacity;
    float fuelEfficiency;
    float mileage;

    Vehicle(string p, string b, string m, int y, string f, float l, float e, float km)
        : plate(p), brand(b), model(m), year(y), fuel(f), loadCapacity(l), fuelEfficiency(e), mileage(km) {}

    void display() const {
        cout << left << setw(10) << plate << setw(10) << brand << setw(10) << model
             << setw(6) << year << setw(10) << fuel << setw(10) << loadCapacity
             << setw(10) << fuelEfficiency << setw(10) << mileage << endl;
    }
};

// I manage the entire fleet including registration and reporting.
class Fleet {
    unordered_map<string, Vehicle> vehicles;

public:
    void addVehicle(const Vehicle& v) {
        vehicles[v.plate] = v;
        cout << "Vehicle added." << endl;
    }

    void listVehicles() const {
        for (const auto& pair : vehicles) {
            pair.second.display();
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        for (const auto& pair : vehicles) {
            const Vehicle& v = pair.second;
            file << v.plate << "," << v.brand << "," << v.model << "," << v.year << ","
                 << v.fuel << "," << v.loadCapacity << "," << v.fuelEfficiency << ","
                 << v.mileage << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string p, b, m, f;
            int y;
            float l, e, km;
            getline(ss, p, ','); getline(ss, b, ','); getline(ss, m, ',');
            ss >> y; ss.ignore(); getline(ss, f, ',');
            ss >> l; ss.ignore(); ss >> e; ss.ignore(); ss >> km;
            vehicles[p] = Vehicle(p, b, m, y, f, l, e, km);
        }
    }
};

// I provide the CLI menu and main control loop.
int main() {
    Fleet fleet;
    fleet.loadFromFile("fleet_data.csv");

    int option;
    do {
        cout << "\nFleet Management System" << endl;
        cout << "1. Add Vehicle\n2. List Vehicles\n3. Save & Exit\nChoose: ";
        cin >> option;
        cin.ignore();

        if (option == 1) {
            string p, b, m, f;
            int y;
            float l, e, km;
            cout << "Plate: "; getline(cin, p);
            cout << "Brand: "; getline(cin, b);
            cout << "Model: "; getline(cin, m);
            cout << "Year: "; cin >> y; cin.ignore();
            cout << "Fuel: "; getline(cin, f);
            cout << "Load Capacity: "; cin >> l;
            cout << "Fuel Efficiency: "; cin >> e;
            cout << "Mileage: "; cin >> km;
            cin.ignore();
            fleet.addVehicle(Vehicle(p, b, m, y, f, l, e, km));
        } else if (option == 2) {
            fleet.listVehicles();
        } else if (option == 3) {
            fleet.saveToFile("fleet_data.csv");
            cout << "Data saved. Exiting..." << endl;
        } else {
            cout << "Invalid option." << endl;
        }
    } while (option != 3);

    return 0;
}
