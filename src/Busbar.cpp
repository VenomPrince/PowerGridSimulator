// Busbar.cpp
#include "../include/Busbar.h"
#include <algorithm>
#include <iostream>

Busbar::Busbar(const std::string& id) : id(id), energized(false) {}

std::string Busbar::getId() const {
    return id;
}

bool Busbar::isEnergized() const {
    return energized;
}

double Busbar::getTotalConnectedLoad() const {
    double totalLoad = 0.0;
    for (const auto& load : connectedLoads) {
        if (load->isLoadConnected()) {
            totalLoad += load->getPowerDemand();
        }
    }
    return totalLoad;
}

double Busbar::getTotalAvailablePower() const {
    double totalPower = 0.0;
    for (const auto& source : connectedSources) {
        if (source->isOperational()) {
            totalPower += source->getAvailableCapacity();
        }
    }
    return totalPower;
}

std::vector<std::shared_ptr<Load>> Busbar::getConnectedLoads() const {
    return connectedLoads;
}

std::vector<std::shared_ptr<PowerSource>> Busbar::getConnectedSources() const {
    return connectedSources;
}

void Busbar::connectLoad(std::shared_ptr<Load> load) {
    connectedLoads.push_back(load);
    load->connect();
}

void Busbar::disconnectLoad(const std::string& loadId) {
    auto it = std::find_if(connectedLoads.begin(), connectedLoads.end(),
                         [&loadId](const std::shared_ptr<Load>& load) {
                             return load->getId() == loadId;
                         });
    
    if (it != connectedLoads.end()) {
        (*it)->disconnect();
        connectedLoads.erase(it);
    }
}

void Busbar::connectSource(std::shared_ptr<PowerSource> source) {
    connectedSources.push_back(source);
    // Check if busbar should be energized
    energized = !connectedSources.empty();
}

void Busbar::disconnectSource(const std::string& sourceId) {
    auto it = std::find_if(connectedSources.begin(), connectedSources.end(),
                         [&sourceId](const std::shared_ptr<PowerSource>& source) {
                             return source->getId() == sourceId;
                         });
    
    if (it != connectedSources.end()) {
        connectedSources.erase(it);
        // Check if busbar is still energized
        energized = !connectedSources.empty();
    }
}

bool Busbar::distributeLoadsToPowerSources() {
    if (connectedSources.empty()) {
        // No power sources, can't distribute
        for (auto& load : connectedLoads) {
            load->setServed(false);
        }
        return false;
    }
    
    // Reset all power sources
    for (auto& source : connectedSources) {
        source->resetLoading();
    }
    
    // Mark all loads as not served initially
    for (auto& load : connectedLoads) {
        load->setServed(false);
    }
    
    // Sort loads by priority (critical first)
    std::vector<std::shared_ptr<Load>> sortedLoads = connectedLoads;
    std::sort(sortedLoads.begin(), sortedLoads.end(), 
              [](const std::shared_ptr<Load>& a, const std::shared_ptr<Load>& b) {
                  return static_cast<int>(a->getPriority()) < static_cast<int>(b->getPriority());
              });
    
    // Try to distribute loads
    bool allLoadsServed = true;
    for (auto& load : sortedLoads) {
        if (!load->isLoadConnected()) continue;
        
        bool loadServed = false;
        double demandPower = load->getPowerDemand();
        
        // Find a source that can handle this load
        for (auto& source : connectedSources) {
            if (source->canSupplyPower(demandPower)) {
                source->addLoad(demandPower);
                load->setServed(true);
                loadServed = true;
                break;
            }
        }
        
        if (!loadServed) {
            allLoadsServed = false;
        }
    }
    
    return allLoadsServed;
}

void Busbar::performLoadShedding() {
    // Reset all power sources
    for (auto& source : connectedSources) {
        source->resetLoading();
    }
    
    // Sort loads by priority (critical first)
    std::vector<std::shared_ptr<Load>> sortedLoads = connectedLoads;
    std::sort(sortedLoads.begin(), sortedLoads.end(), 
              [](const std::shared_ptr<Load>& a, const std::shared_ptr<Load>& b) {
                  return static_cast<int>(a->getPriority()) < static_cast<int>(b->getPriority());
              });
    
    // Try to distribute loads by priority
    for (auto& load : sortedLoads) {
        if (!load->isLoadConnected()) continue;
        
        double demandPower = load->getPowerDemand();
        bool loadServed = false;
        
        // Find source with available capacity
        for (auto& source : connectedSources) {
            if (source->canSupplyPower(demandPower)) {
                source->addLoad(demandPower);
                load->setServed(true);
                loadServed = true;
                break;
            }
        }
        
        if (!loadServed) {
            // This load cannot be served (it will be shed)
            load->setServed(false);
            std::cout << "Load shedding: " << load->getId() << " (" << load->getTypeString() 
                      << ", " << load->getPowerDemand() << " kW) was shed.\n";
        }
    }
}
