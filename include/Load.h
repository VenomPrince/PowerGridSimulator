// Load.h
#ifndef LOAD_H
#define LOAD_H

#include <string>

// Enum for different load types
enum class LoadType {
    RESIDENTIAL,
    COMMERCIAL,
    INDUSTRIAL,
    CRITICAL  // e.g., hospitals, emergency services
};

// Priority levels (1 highest, 5 lowest)
enum class Priority {
    CRITICAL = 1,  // Must never be shed (e.g., hospitals)
    HIGH = 2,      // Shed only in severe emergencies
    MEDIUM = 3,    // Normal loads
    LOW = 4,       // Non-essential loads
    MINIMAL = 5    // First to be shed (e.g., decorative lighting)
};

class Load {
private:
    std::string id;
    double powerDemand;  // in kW
    LoadType type;
    Priority priority;
    bool isConnected;    // Whether the load is currently connected
    bool isServed;       // Whether the load is currently being supplied power

public:
    // Constructor
    Load(const std::string& id, double powerDemand, LoadType type, Priority priority);
    
    // Getters
    std::string getId() const;
    double getPowerDemand() const;
    LoadType getType() const;
    Priority getPriority() const;
    bool isLoadConnected() const;
    bool isLoadServed() const;
    
    // Setters
    void setPowerDemand(double demand);
    void connect();
    void disconnect();
    void setServed(bool served);
    
    // Utility functions
    std::string getTypeString() const;
    std::string getPriorityString() const;
};

#endif // LOAD_H
