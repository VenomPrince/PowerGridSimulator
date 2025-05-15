// Grid.cpp
#include "../include/Grid.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Grid::Grid(const std::string& name) : name(name), totalDemand(0.0), totalSupply(0.0), 
                                      servedDemand(0.0), shedLoad(0.0) {}

void Grid::addBusbar(std::shared_ptr<Busbar> busbar) {
    busbars.push_back(busbar);
}

void Grid::removeBusbar(const std::string& busbarId) {
    auto it = std::find_if(busbars.begin(), busbars.end(),
                          [&busbarId](const std::shared_ptr<Busbar>& busbar) {
                              return busbar->getId() == busbarId;
                          });
    
    if (it != busbars.end()) {
        // Remove all loads and sources from the busbar
        auto loads = (*it)->getConnectedLoads();
        for (const auto& load : loads) {
            allLoads.erase(load->getId());
        }
        
        auto sources = (*it)->getConnectedSources();
        for (const auto& source : sources) {
            allSources.erase(source->getId());
        }
        
        busbars.erase(it);
    }
}

std::shared_ptr<Busbar> Grid::getBusbar(const std::string& busbarId) {
    auto it = std::find_if(busbars.begin(), busbars.end(),
                          [&busbarId](const std::shared_ptr<Busbar>& busbar) {
                              return busbar->getId() == busbarId;
                          });
    
    return (it != busbars.end()) ? *it : nullptr;
}

void Grid::addLoad(std::shared_ptr<Load> load, const std::string& busbarId) {
    auto busbar = getBusbar(busbarId);
    if (busbar) {
        busbar->connectLoad(load);
        allLoads[load->getId()] = load;
    } else {
        std::cout << "Error: Busbar " << busbarId << " not found.\n";
    }
}

void Grid::removeLoad(const std::string& loadId) {
    auto loadIt = allLoads.find(loadId);
    if (loadIt != allLoads.end()) {
        // Find busbar that contains this load
        for (auto& busbar : busbars) {
            busbar->disconnectLoad(loadId);
        }
        allLoads.erase(loadId);
    } else {
        std::cout << "Error: Load " << loadId << " not found.\n";
    }
}

std::shared_ptr<Load> Grid::getLoad(const std::string& loadId) {
    auto it = allLoads.find(loadId);
    return (it != allLoads.end()) ? it->second : nullptr;
}

void Grid::addSource(std::shared_ptr<PowerSource> source, const std::string& busbarId) {
    auto busbar = getBusbar(busbarId);
    if (busbar) {
        busbar->connectSource(source);
        allSources[source->getId()] = source;
    } else {
        std::cout << "Error: Busbar " << busbarId << " not found.\n";
    }
}

void Grid::removeSource(const std::string& sourceId) {
    auto sourceIt = allSources.find(sourceId);
    if (sourceIt != allSources.end()) {
        // Find busbar that contains this source
        for (auto& busbar : busbars) {
            busbar->disconnectSource(sourceId);
        }
        allSources.erase(sourceId);
    } else {
        std::cout << "Error: Power Source " << sourceId << " not found.\n";
    }
}

std::shared_ptr<PowerSource> Grid::getSource(const std::string& sourceId) {
    auto it = allSources.find(sourceId);
    return (it != allSources.end()) ? it->second : nullptr;
}

void Grid::distributeLoadOptimally() {
    // Reset all power sources
    for (auto& sourcePair : allSources) {
        sourcePair.second->resetLoading();
    }
    
    // Reset load service status
    for (auto& loadPair : allLoads) {
        loadPair.second->setServed(false);
    }
    
    // First, try to distribute loads on each busbar
    for (auto& busbar : busbars) {
        if (!busbar->distributeLoadsToPowerSources()) {
            // If not all loads could be served, perform load shedding
            busbar->performLoadShedding();
        }
    }
    
    // Update statistics
    updateStatistics();
}

void Grid::performSystemWideLoadShedding() {
    // Reset all power sources
    for (auto& sourcePair : allSources) {
        sourcePair.second->resetLoading();
    }
    
    // Reset load service status
    for (auto& loadPair : allLoads) {
        loadPair.second->setServed(false);
    }
    
    // Collect all loads across the system
    std::vector<std::shared_ptr<Load>> allLoadsList;
    for (auto& loadPair : allLoads) {
        if (loadPair.second->isLoadConnected()) {
            allLoadsList.push_back(loadPair.second);
        }
    }
    
    // Sort loads by priority (critical first)
    std::sort(allLoadsList.begin(), allLoadsList.end(), 
              [](const std::shared_ptr<Load>& a, const std::shared_ptr<Load>& b) {
                  return static_cast<int>(a->getPriority()) < static_cast<int>(b->getPriority());
              });
    
    // Try to serve loads by priority
    for (auto& load : allLoadsList) {
        double demandPower = load->getPowerDemand();
        bool loadServed = false;
        
        // Find any source in the system that can handle this load
        for (auto& sourcePair : allSources) {
            auto& source = sourcePair.second;
            if (source->isOperational() && source->canSupplyPower(demandPower)) {
                source->addLoad(demandPower);
                load->setServed(true);
                loadServed = true;
                break;
            }
        }
        
        if (!loadServed) {
            // This load cannot be served (it will be shed)
            load->setServed(false);
            std::cout << "System-wide load shedding: " << load->getId() << " (" 
                      << load->getTypeString() << ", " << load->getPowerDemand() 
                      << " kW) was shed.\n";
        }
    }
    
    // Update statistics
    updateStatistics();
}

void Grid::updateStatistics() {
    totalDemand = 0.0;
    servedDemand = 0.0;
    totalSupply = 0.0;
    
    // Calculate total demand and served demand
    for (auto& loadPair : allLoads) {
        if (loadPair.second->isLoadConnected()) {
            double demand = loadPair.second->getPowerDemand();
            totalDemand += demand;
            
            if (loadPair.second->isLoadServed()) {
                servedDemand += demand;
            }
        }
    }
    
    // Calculate total supply
    for (auto& sourcePair : allSources) {
        if (sourcePair.second->isOperational()) {
            totalSupply += sourcePair.second->getCapacity();
        }
    }
    
    // Calculate shed load
    shedLoad = totalDemand - servedDemand;
}

double Grid::getTotalDemand() const {
    return totalDemand;
}

double Grid::getTotalSupply() const {
    return totalSupply;
}

double Grid::getServedDemand() const {
    return servedDemand;
}

double Grid::getShedLoad() const {
    return shedLoad;
}

double Grid::getSupplyUtilizationPercent() const {
    if (totalSupply > 0.0) {
        return (servedDemand / totalSupply) * 100.0;
    }
    return 0.0;
}

void Grid::printSystemReport() const {
    std::cout << "\n======= " << name << " SYSTEM REPORT =======\n";
    
    // Print busbars and their status
    std::cout << "\nBUSBARS:\n";
    std::cout << std::left << std::setw(15) << "ID" 
              << std::setw(10) << "Status" 
              << std::setw(15) << "Connected Load" 
              << std::setw(20) << "Available Power" << "\n";
    std::cout << std::string(60, '-') << "\n";
    
    for (const auto& busbar : busbars) {
        std::cout << std::left << std::setw(15) << busbar->getId() 
                  << std::setw(10) << (busbar->isEnergized() ? "Energized" : "De-energized") 
                  << std::setw(15) << busbar->getTotalConnectedLoad() << " kW" 
                  << std::setw(20) << busbar->getTotalAvailablePower() << " kW" << "\n";
    }
    
    // Print power sources
    std::cout << "\nPOWER SOURCES:\n";
    std::cout << std::left << std::setw(15) << "ID" 
              << std::setw(10) << "Status" 
              << std::setw(15) << "Capacity" 
              << std::setw(15) << "Current Load" 
              << std::setw(20) << "Available Capacity" << "\n";
    std::cout << std::string(75, '-') << "\n";
    
    for (const auto& sourcePair : allSources) {
        const auto& source = sourcePair.second;
        std::cout << std::left << std::setw(15) << source->getId() 
                  << std::setw(10) << (source->isOperational() ? "Online" : "Offline") 
                  << std::setw(15) << source->getCapacity() << " kW" 
                  << std::setw(15) << source->getCurrentLoad() << " kW"
                  << std::setw(20) << source->getAvailableCapacity() << " kW" << "\n";
    }
    
    // Print loads
    std::cout << "\nLOADS:\n";
    std::cout << std::left << std::setw(15) << "ID" 
              << std::setw(15) << "Type" 
              << std::setw(15) << "Priority" 
              << std::setw(15) << "Demand" 
              << std::setw(10) << "Connected" 
              << std::setw(10) << "Served" << "\n";
    std::cout << std::string(80, '-') << "\n";
    
    for (const auto& loadPair : allLoads) {
        const auto& load = loadPair.second;
        std::cout << std::left << std::setw(15) << load->getId() 
                  << std::setw(15) << load->getTypeString() 
                  << std::setw(15) << load->getPriorityString() 
                  << std::setw(15) << load->getPowerDemand() << " kW"
                  << std::setw(10) << (load->isLoadConnected() ? "Yes" : "No") 
                  << std::setw(10) << (load->isLoadServed() ? "Yes" : "No") << "\n";
    }
    
    // Print system statistics
    std::cout << "\nSYSTEM STATISTICS:\n";
    std::cout << "Total Supply Capacity: " << totalSupply << " kW\n";
    std::cout << "Total Connected Load: " << totalDemand << " kW\n";
    std::cout << "Total Served Load: " << servedDemand << " kW\n";
    std::cout << "Total Shed Load: " << shedLoad << " kW\n";
    
    double supplyUtilization = getSupplyUtilizationPercent();
    std::cout << "Supply Utilization: " << supplyUtilization << "%\n";
    
    if (totalDemand > 0.0) {
        double serviceLevel = (servedDemand / totalDemand) * 100.0;
        std::cout << "Service Level: " << serviceLevel << "%\n";
    } else {
        std::cout << "Service Level: N/A (no demand)\n";
    }
    
    std::cout << "=================================\n\n";
}