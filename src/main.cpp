// main.cpp
#include <iostream>
#include "../include/Simulator.h"

int main() {
    std::cout << "==================================\n";
    std::cout << "Power Distribution & Load Management Simulator\n";
    std::cout << "Demo Version\n";
    std::cout << "==================================\n\n";
    
    Simulator simulator;
    simulator.runInteractiveSimulation();
    
    return 0;
}
