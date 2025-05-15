# Power Distribution & Load Management Simulator
## Build & Run Instructions

### Prerequisites
- C++ compiler with C++17 support (GCC, Clang, MSVC)
- CMake (version 3.10 or higher)

### Building the Project

#### Linux / macOS
1. Clone or download the repository
2. Open a terminal in the project directory
3. Create a build directory and navigate to it:
```bash
mkdir build
cd build
```
4. Generate the build files using CMake:
```bash
cmake ..
```
5. Build the project:
```bash
make
```
6. Run the simulator:
```bash
./bin/PowerGridSimulator
```

#### Windows
1. Clone or download the repository
2. Open a command prompt or PowerShell in the project directory
3. Create a build directory and navigate to it:
```cmd
mkdir build
cd build
```
4. Generate the build files using CMake:
```cmd
cmake .. -G "Visual Studio 17 2022"  # Adjust for your VS version
```
5. Build the project:
```cmd
cmake --build . --config Release
```
6. Run the simulator:
```cmd
.\bin\Release\PowerGridSimulator.exe
```

### Alternative: Using an IDE
You can also import the project into an IDE that supports CMake projects:
- Visual Studio with CMake support
- CLion
- Visual Studio Code with CMake Tools extension

### Demo Usage
When you run the simulator, a default scenario will be loaded with:
- 2 busbars (Main and Secondary)
- 3 power sources (2 generators and 1 transformer)
- 5 loads of different types and priorities

You can interact with the simulator through the menu to:
1. Add new loads
2. Remove existing loads
3. Add new power sources
4. Modify power source capacity or status
5. Run simulation steps
6. View system reports

The simulator demonstrates:
- Basic load distribution
- Priority-based load shedding when demand exceeds supply
- Real-time system state monitoring

### Project Structure
- `include/`: Header files
- `src/`: Source files
- `CMakeLists.txt`: Build configuration
