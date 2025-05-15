// Busbar.h
#ifndef BUSBAR_H
#define BUSBAR_H

#include <string>
#include <vector>
#include <memory>
#include "Load.h"
#include "PowerSource.h"

class Busbar {
private:
    std::string id;
    std::vector<std::shared_ptr<Load>> connectedLoads;
    std::vector<std::shared_ptr<PowerSource>> connectedSources;
    bool energized;  // Whether the busbar is energized

public:
    // Constructor
    Busbar(const std::string& id);
    
    // Getters
    std::string getId() const;
    bool isEnergized() const;
    double getTotalConnectedLoad() const;
    double getTotalAvailablePower() const;
    std::vector<std::shared_ptr<Load>> getConnectedLoads() const;
    std::vector<std::shared_ptr<PowerSource>> getConnectedSources() const;
    
    // Connection management
    void connectLoad(std::shared_ptr<Load> load);
    void disconnectLoad(const std::string& loadId);
    void connectSource(std::shared_ptr<PowerSource> source);
    void disconnectSource(const std::string& sourceId);
    
    // Power distribution
    bool distributeLoadsToPowerSources();
    void performLoadShedding();
};

#endif // BUSBAR_H
