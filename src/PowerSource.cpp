// PowerSource.cpp
#include "../include/PowerSource.h"

PowerSource::PowerSource(const std::string& id, double capacity)
    : id(id), capacity(capacity), currentLoad(0.0), operational(true) {}

std::string PowerSource::getId() const {
    return id;
}

double PowerSource::getCapacity() const {
    return capacity;
}

double PowerSource::getCurrentLoad() const {
    return currentLoad;
}

double PowerSource::getAvailableCapacity() const {
    if (!operational) return 0.0;
    return capacity - currentLoad;
}

bool PowerSource::isOperational() const {
    return operational;
}

void PowerSource::setCapacity(double newCapacity) {
    capacity = newCapacity;
}

void PowerSource::setOperational(bool isOperational) {
    operational = isOperational;
}

bool PowerSource::canSupplyPower(double requestedPower) const {
    if (!operational) return false;
    return (currentLoad + requestedPower <= capacity);
}

bool PowerSource::addLoad(double power) {
    if (!operational || !canSupplyPower(power)) {
        return false;
    }
    
    currentLoad += power;
    return true;
}

void PowerSource::removeLoad(double power) {
    currentLoad -= power;
    if (currentLoad < 0.0) {
        currentLoad = 0.0;
    }
}

void PowerSource::resetLoading() {
    currentLoad = 0.0;
}
