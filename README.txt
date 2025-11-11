================================================================================
CELLULAR NETWORK SIMULATOR
OOPD Project - Monsoon 2025
================================================================================

PROJECT DESCRIPTION:
-------------------
This project implements a cellular network simulator that models 2G, 3G, 4G, 
and 5G communication technologies. It demonstrates various object-oriented 
programming concepts including inheritance, polymorphism, data abstraction, 
templates, and exception handling.

AUTHOR INFORMATION:
------------------
[Add your name(s) here]
[Add your roll number(s) here]

FILES INCLUDED:
--------------
1. CellularNetwork.h      - Header file with class declarations
2. CellularNetwork.cpp    - Implementation of cellular network classes
3. main.cpp               - Main program entry point
4. basicIO.h              - Basic I/O interface (provided)
5. basicIO.cpp            - Basic I/O implementation (provided)
6. syscall.s              - Assembly code for system calls
7. Makefile               - Build automation file
8. README.txt             - This file

BUILD INSTRUCTIONS:
------------------

Prerequisites:
- g++ compiler with C++17 support
- GNU Make
- Linux/Unix environment (for syscall.s)

Building the Project:

1. To build both debug and release versions:
   $ make all

2. To build only the debug version:
   $ make debug

3. To build only the release (optimized) version:
   $ make release

4. To clean all build artifacts:
   $ make clean

Output Binaries:
- Debug version: cellular_network_debug
- Release version: cellular_network

RUNNING THE SIMULATOR:
---------------------

1. Run the debug version:
   $ make run-debug
   OR
   $ ./cellular_network_debug

2. Run the release version:
   $ make run
   OR
   $ ./cellular_network

The simulator will automatically run all four simulations (2G, 3G, 4G, 5G) 
and display the results.

INPUT FILE FORMAT:
-----------------
This version of the simulator does not require input files. All simulation 
parameters are hardcoded according to the project specifications:

2G Simulation:
- Bandwidth: 1 MHz
- Channel size: 200 kHz
- Users per channel: 16
- Messages per user: 20 (5 data + 15 voice)

3G Simulation:
- Bandwidth: 1 MHz
- Channel size: 200 kHz
- Users per channel: 32
- Messages per user: 10

4G Simulation:
- Bandwidth: 1 MHz
- Channel size: 10 kHz
- Users per channel: 30
- Antennas: 4
- Messages per user: 10

5G Simulation:
- Primary bandwidth: 1 MHz
- Additional bandwidth: 10 MHz at 1800 MHz
- Channel size: 10 kHz (primary), 1 MHz (1800 MHz band)
- Users per channel: 30 (10 kHz), 30 per MHz (1800 MHz)
- Antennas: 16 (Massive MIMO)
- Messages per user: 10

OUTPUT DESCRIPTION:
------------------
For each generation (2G, 3G, 4G, 5G), the simulator displays:
1. Technology details (TDMA, CDMA, OFDM, etc.)
2. Bandwidth configuration
3. Total capacity (number of users supported)
4. Users occupying the first channel
5. Number of cellular cores needed

OBJECT-ORIENTED FEATURES DEMONSTRATED:
------------------------------------

1. Inheritance:
   - UserDevice is a base class
   - User2G, User3G, User4G, User5G inherit from UserDevice
   - CellTower is a base class
   - Tower2G, Tower3G, Tower4G, Tower5G inherit from CellTower

2. Polymorphism:
   - Virtual function getMessagesGenerated() in UserDevice
   - Overridden in all derived user classes
   - Virtual function getTotalCapacity() in CellTower

3. Data Abstraction & Encapsulation:
   - Private member variables with public getter methods
   - Protected members in base classes

4. Data Hiding:
   - Private members not accessible outside class
   - Public interface through methods

5. Templates:
   - NetworkContainer<T> template class for type-safe collections

6. Exception Handling:
   - NetworkException base class
   - CapacityExceededException for capacity violations
   - InvalidConfigurationException for configuration errors
   - Try-catch blocks in simulator

CODE ATTRIBUTION:
----------------
- basicIO.h, basicIO.cpp: Provided by course instructor
- syscall.s: Standard x86-64 system call wrapper
- All other code: Original implementation for this project

KNOWN LIMITATIONS:
-----------------
1. User assignment to channels is simplified for demonstration
2. Cellular core overhead calculation uses simplified model
3. No persistent storage of simulation state
4. Fixed simulation parameters (not configurable via input files)

TESTING:
-------
The program has been tested with:
- Compilation on g++ with -std=c++17
- Both debug and release builds
- Exception handling scenarios
- Capacity validation

COMPILATION FLAGS:
-----------------
Debug build:
  -std=c++17 -Wall -Wextra -g -O0 -DDEBUG

Release build:
  -std=c++17 -Wall -Wextra -O3 -DNDEBUG

TROUBLESHOOTING:
---------------
If you encounter build errors:
1. Ensure g++ supports C++17 (g++ --version should show >= 7.0)
2. Make sure all files are in the same directory
3. Check that syscall.s is compatible with your architecture (x86-64)
4. Try cleaning and rebuilding: make clean && make all

For runtime errors:
1. Use the debug build for better error messages
2. Check that basic I/O operations are working correctly
3. Verify system call compatibility on your platform

CONTACT:
-------
For questions or issues, please contact:
[Add your email here]

================================================================================
END OF README
================================================================================
