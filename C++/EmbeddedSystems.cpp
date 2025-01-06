#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <functional>

using namespace std;

// Class representing a temperature sensor in the IoT device
class TemperatureSensor {
private:
    default_random_engine engine;               // Random engine to simulate fluctuating temperatures
    uniform_real_distribution<double> tempDist;  // Temperature distribution range

public:
    // Constructor initializes the temperature sensor with a given temperature range
    TemperatureSensor(double minTemp, double maxTemp)
        : tempDist(minTemp, maxTemp), engine(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count())) {}

    // Get the current temperature reading from the sensor
    double getCurrentTemperature() {
        return tempDist(engine);  // Return a random temperature value within the specified range
    }
};

// Class representing an HVAC (Heating, Ventilation, and Air Conditioning) system
class HVACSystem {
private:
    double minTemp;  // Minimum desired temperature for the house
    double maxTemp;  // Maximum desired temperature for the house

public:
    // Constructor initializes the HVAC system with temperature thresholds
    HVACSystem(double minTemp, double maxTemp) : minTemp(minTemp), maxTemp(maxTemp) {}

    // Control the HVAC system based on the current temperature
    void controlTemperature(double currentTemp) {
        if (currentTemp < minTemp) {
            cout << "Current temperature: " << currentTemp << "°C. Heating system ON.\n";
            // Simulate heating action
        } else if (currentTemp > maxTemp) {
            cout << "Current temperature: " << currentTemp << "°C. Cooling system ON.\n";
            // Simulate cooling action
        } else {
            cout << "Current temperature: " << currentTemp << "°C. Temperature is within the desired range.\n";
        }
    }
};

// Class representing the smart house system
class SmartHouse {
private:
    TemperatureSensor sensor;   // Temperature sensor object
    HVACSystem hvac;            // HVAC system object

public:
    // Constructor initializes the smart house with a temperature sensor and HVAC system
    SmartHouse(double minTemp, double maxTemp)
        : sensor(minTemp, maxTemp), hvac(minTemp, maxTemp) {}

    // Simulate the real-time temperature control system
    void monitorAndControl() {
        while (true) {
            // Get the current temperature from the sensor
            double currentTemp = sensor.getCurrentTemperature();
            cout << "Current room temperature: " << currentTemp << "°C\n";

            // Control the temperature based on the current reading
            hvac.controlTemperature(currentTemp);

            // Simulate waiting for the next temperature reading (in real applications, this would be real-time data)
            this_thread::sleep_for(chrono::seconds(3));
        }
    }
};

// Main function to run the temperature control system in the smart house
int main() {
    // Initialize the smart house with desired temperature range (e.g., 18°C to 24°C)
    SmartHouse house(18.0, 24.0);

    // Start monitoring and controlling the temperature
    house.monitorAndControl();

    return 0;
}