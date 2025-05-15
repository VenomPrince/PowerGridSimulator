// PowerSource.h
#ifndef POWER_SOURCE_H
#define POWER_SOURCE_H

#include <string>
#include <vector>
#include "Load.h"

class PowerSource {
private:
    std::string id;
    double capacity;         // Maximum power in kW
    double currentLoad;      // Current load in kW
    bool operational;        // Whether the source is operational

public:
    // Constructor
    PowerSource(const std::string& id, double capacity);
    
    // Getters
    std::string getId() const;
    double getCapacity() const;
    double getCurrentLoad() const;
    double getAvailableCapacity() const;
    bool isOperational() const;
    
    // Setters
    void setCapacity(double newCapacity);
    void setOperational(bool isOperational);
    
    // Operation functions
    bool canSupplyPower(double requestedPower) const;
    bool addLoad(double power);
    void removeLoad(double power);
    void resetLoading();
};

#endif // POWER_SOURCE_H
