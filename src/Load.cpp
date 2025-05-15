// Load.cpp
#include "../include/Load.h"

Load::Load(const std::string& id, double powerDemand, LoadType type, Priority priority)
    : id(id), powerDemand(powerDemand), type(type), priority(priority), 
      isConnected(false), isServed(false) {}

std::string Load::getId() const {
    return id;
}

double Load::getPowerDemand() const {
    return powerDemand;
}

LoadType Load::getType() const {
    return type;
}

Priority Load::getPriority() const {
    return priority;
}

bool Load::isLoadConnected() const {
    return isConnected;
}

bool Load::isLoadServed() const {
    return isServed;
}

void Load::setPowerDemand(double demand) {
    powerDemand = demand;
}

void Load::connect() {
    isConnected = true;
}

void Load::disconnect() {
    isConnected = false;
    isServed = false;
}

void Load::setServed(bool served) {
    isServed = served;
}

std::string Load::getTypeString() const {
    switch (type) {
        case LoadType::RESIDENTIAL: return "Residential";
        case LoadType::COMMERCIAL: return "Commercial";
        case LoadType::INDUSTRIAL: return "Industrial";
        case LoadType::CRITICAL: return "Critical";
        default: return "Unknown";
    }
}

std::string Load::getPriorityString() const {
    switch (priority) {
        case Priority::CRITICAL: return "Critical (1)";
        case Priority::HIGH: return "High (2)";
        case Priority::MEDIUM: return "Medium (3)";
        case Priority::LOW: return "Low (4)";
        case Priority::MINIMAL: return "Minimal (5)";
        default: return "Unknown";
    }
}
