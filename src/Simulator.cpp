// Simulator.cpp
#include "../include/Simulator.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

Simulator::Simulator() : currentTimeStep(0), running(false) {
    grid = std::make_shared<Grid>("Demo Power Grid");
}

void Simulator::setupDefaultScenario() {
    // Create busbars
    auto mainBusbar = std::make_shared<Busbar>("Main");
    auto secondaryBusbar = std::make_shared<Busbar>("Secondary");
    
    grid->addBusbar(mainBusbar);
    grid->addBusbar(secondaryBusbar);
    
    // Create power sources
    auto generator1 = std::make_shared<PowerSource>("GEN-1", 1000.0);  // 1000 kW generator
    auto generator2 = std::make_shared<PowerSource>("GEN-2", 500.0);   // 500 kW generator
    auto transformer = std::make_shared<PowerSource>("TR-1", 1500.0);  // 1500 kW transformer
    
    grid->addSource(generator1, "Main");
    grid->addSource(generator2, "Secondary");
    grid->addSource(transformer, "Main");
    
    // Create loads
    auto hospital = std::make_shared<Load>("HOSP-1", 600.0, LoadType::CRITICAL, Priority::CRITICAL);
    auto factory = std::make_shared<Load>("FACT-1", 800.0, LoadType::INDUSTRIAL, Priority::MEDIUM);
    auto residential = std::make_shared<Load>("RES-1", 400.0, LoadType::RESIDENTIAL, Priority::LOW);
    auto commercial = std::make_shared<Load>("COMM-1", 300.0, LoadType::COMMERCIAL, Priority::MEDIUM);
    auto streetlights = std::make_shared<Load>("STLT-1", 100.0, LoadType::COMMERCIAL, Priority::MINIMAL);
    
    grid->addLoad(hospital, "Main");
    grid->addLoad(factory, "Main");
    grid->addLoad(residential, "Secondary");
    grid->addLoad(commercial, "Secondary");
    grid->addLoad(streetlights, "Secondary");
    
    // Distribute loads
    grid->distributeLoadOptimally();
    
    std::cout << "Default scenario set up.\n";
    grid->printSystemReport();
}

void Simulator::run() {
    running = true;
    std::cout << "Simulation running...\n";
}

void Simulator::pause() {
    running = false;
    std::cout << "Simulation paused.\n";
}

void Simulator::stop() {
    running = false;
    currentTimeStep = 0;
    std::cout << "Simulation stopped and reset.\n";
}

void Simulator::displayMenu() const {
    std::cout << "\n==== POWER GRID SIMULATOR MENU ====\n";
    std::cout << "1. Add a new load\n";
    std::cout << "2. Remove a load\n";
    std::cout << "3. Add a new power source\n";
    std::cout << "4. Modify power source (capacity/status)\n";
    std::cout << "5. Run simulation step\n";
    std::cout << "6. Print system report\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

void Simulator::addLoadInteractive() {
    std::string id, busbarId;
    double power;
    int typeChoice, priorityChoice;
    
    std::cout << "Enter load ID: ";
    std::cin >> id;
    
    std::cout << "Enter load power demand (kW): ";
    std::cin >> power;
    
    std::cout << "Select load type:\n";
    std::cout << "1. Residential\n";
    std::cout << "2. Commercial\n";
    std::cout << "3. Industrial\n";
    std::cout << "4. Critical\n";
    std::cout << "Enter choice: ";
    std::cin >> typeChoice;
    
    LoadType type;
    switch (typeChoice) {
        case 1: type = LoadType::RESIDENTIAL; break;
        case 2: type = LoadType::COMMERCIAL; break;
        case 3: type = LoadType::INDUSTRIAL; break;
        case 4: type = LoadType::CRITICAL; break;
        default: type = LoadType::RESIDENTIAL;
    }
    
    std::cout << "Select load priority:\n";
    std::cout << "1. Critical (never shed)\n";
    std::cout << "2. High\n";
    std::cout << "3. Medium\n";
    std::cout << "4. Low\n";
    std::cout << "5. Minimal (shed first)\n";
    std::cout << "Enter choice: ";
    std::cin >> priorityChoice;
    
    Priority priority;
    switch (priorityChoice) {
        case 1: priority = Priority::CRITICAL; break;
        case 2: priority = Priority::HIGH; break;
        case 3: priority = Priority::MEDIUM; break;
        case 4: priority = Priority::LOW; break;
        case 5: priority = Priority::MINIMAL; break;
        default: priority = Priority::MEDIUM;
    }
    
    std::cout << "Enter busbar ID to connect to: ";
    std::cin >> busbarId;
    
    auto load = std::make_shared<Load>(id, power, type, priority);
    grid->addLoad(load, busbarId);
    
    std::cout << "Load " << id << " added to busbar " << busbarId << ".\n";
    
    // Re-distribute loads
    grid->distributeLoadOptimally();
}

void Simulator::removeLoadInteractive() {
    std::string id;
    
    std::cout << "Enter ID of load to remove: ";
    std::cin >> id;
    
    grid->removeLoad(id);
    std::cout << "Load " << id << " removed.\n";
    
    // Re-distribute loads
    grid->distributeLoadOptimally();
}

void Simulator::addSourceInteractive() {
    std::string id, busbarId;
    double capacity;
    
    std::cout << "Enter power source ID: ";
    std::cin >> id;
    
    std::cout << "Enter power source capacity (kW): ";
    std::cin >> capacity;
    
    std::cout << "Enter busbar ID to connect to: ";
    std::cin >> busbarId;
    
    auto source = std::make_shared<PowerSource>(id, capacity);
    grid->addSource(source, busbarId);
    
    std::cout << "Power source " << id << " added to busbar " << busbarId << ".\n";
    
    // Re-distribute loads
    grid->distributeLoadOptimally();
}

void Simulator::modifySourceInteractive() {
    std::string id;
    int choice;
    
    std::cout << "Enter ID of power source to modify: ";
    std::cin >> id;
    
    auto source = grid->getSource(id);
    if (!source) {
        std::cout << "Source not found.\n";
        return;
    }
    
    std::cout << "What do you want to modify?\n";
    std::cout << "1. Capacity\n";
    std::cout << "2. Operational status\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;
    
    if (choice == 1) {
        double newCapacity;
        std::cout << "Enter new capacity (kW): ";
        std::cin >> newCapacity;
        source->setCapacity(newCapacity);
        std::cout << "Capacity updated.\n";
    }
    else if (choice == 2) {
        int status;
        std::cout << "Set status (1=Operational, 0=Offline): ";
        std::cin >> status;
        source->setOperational(status == 1);
        std::cout << "Status updated.\n";
    }
    
    // Re-distribute loads
    grid->distributeLoadOptimally();
}

void Simulator::simulationStep() {
    currentTimeStep++;
    std::cout << "\n--- Simulation Step " << currentTimeStep << " ---\n";
    
    // Redistribute loads
    grid->distributeLoadOptimally();
    grid->printSystemReport();
}

void Simulator::processUserInput() {
    int choice;
    std::cin >> choice;
    
    // Clear the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            addLoadInteractive();
            break;
        case 2:
            removeLoadInteractive();
            break;
        case 3:
            addSourceInteractive();
            break;
        case 4:
            modifySourceInteractive();
            break;
        case 5:
            simulationStep();
            break;
        case 6:
            grid->printSystemReport();
            break;
        case 7:
            running = false;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
    }
}

void Simulator::runInteractiveSimulation() {
    setupDefaultScenario();
    running = true;
    
    while (running) {
        displayMenu();
        processUserInput();
        
        // Small delay to make the simulation more real-time like
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "Simulation ended.\n";
}
