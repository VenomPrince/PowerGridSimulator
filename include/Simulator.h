// Simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <memory>
#include <string>
#include "Grid.h"

class Simulator {
private:
    std::shared_ptr<Grid> grid;
    int currentTimeStep;
    bool running;
    
    // Helper methods for CLI
    void displayMenu() const;
    void addLoadInteractive();
    void removeLoadInteractive();
    void addSourceInteractive();
    void modifySourceInteractive();
    void simulationStep();

public:
    // Constructor
    Simulator();
    
    // Simulation control
    void setupDefaultScenario();
    void run();
    void pause();
    void stop();
    
    // User interaction through CLI
    void processUserInput();
    void runInteractiveSimulation();
};

#endif // SIMULATOR_H
