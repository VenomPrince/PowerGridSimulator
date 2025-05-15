// Grid.h
#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Busbar.h"
#include "Load.h"
#include "PowerSource.h"

class Grid {
private:
    std::string name;
    std::vector<std::shared_ptr<Busbar>> busbars;
    std::map<std::string, std::shared_ptr<Load>> allLoads;
    std::map<std::string, std::shared_ptr<PowerSource>> allSources;
    
    // Statistics
    double totalDemand;
    double totalSupply;
    double servedDemand;
    double shedLoad;

public:
    // Constructor
    Grid(const std::string& name);
    
    // Grid structure management
    void addBusbar(std::shared_ptr<Busbar> busbar);
    void removeBusbar(const std::string& busbarId);
    std::shared_ptr<Busbar> getBusbar(const std::string& busbarId);
    
    // Load management
    void addLoad(std::shared_ptr<Load> load, const std::string& busbarId);
    void removeLoad(const std::string& loadId);
    std::shared_ptr<Load> getLoad(const std::string& loadId);
    
    // Source management
    void addSource(std::shared_ptr<PowerSource> source, const std::string& busbarId);
    void removeSource(const std::string& sourceId);
    std::shared_ptr<PowerSource> getSource(const std::string& sourceId);
    
    // Power distribution and load shedding
    void distributeLoadOptimally();
    void performSystemWideLoadShedding();
    
    // Statistics and reporting
    void updateStatistics();
    double getTotalDemand() const;
    double getTotalSupply() const;
    double getServedDemand() const;
    double getShedLoad() const;
    double getSupplyUtilizationPercent() const;
    
    // System report
    void printSystemReport() const;
};

#endif // GRID_H
