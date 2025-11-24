#include "CellularNetwork.h"
#include "basicIO.h"

void printHeader() {
    io.outputstring("=================================================");
    io.terminate();
    io.outputstring(" CELLULAR NETWORK SIMULATOR - MENU");
    io.terminate();
    io.outputstring(" OOPD Project - Monsoon 2025");
    io.terminate();
    io.outputstring("=================================================");
    io.terminate();
}

void printMenu() {
    io.outputstring("\nSelect an option:\n");
    io.outputstring(" 1. Simulate 2G\n");
    io.outputstring(" 2. Simulate 3G\n");
    io.outputstring(" 3. Simulate 4G\n");
    io.outputstring(" 4. Simulate 5G\n");
    io.outputstring(" 5. Simulate ALL (2G + 3G + 4G + 5G)\n");
    io.outputstring(" 0. Exit\n");
    io.outputstring("Enter choice: ");
}

int main() {
    try {
        CellularNetworkSimulator simulator;
        bool running = true;
        printHeader();

        while (running) {
            printMenu();
            char buf[32];
            io.inputstring(buf, 32);
            
            int choice = atoi(buf);

            // If atoi gives 0 but input wasn't actually "0", treat as invalid (map to 6)
            if (choice == 0 && buf[0] != '0')
                choice = 6;

            switch (choice) {
                case 1:
                    simulator.simulate2G();
                    break;
                case 2:
                    simulator.simulate3G();
                    break;
                case 3:
                    simulator.simulate4G();
                    break;
                case 4:
                    simulator.simulate5G();
                    break;
                case 5:
                    // simulate all in sequence
                    simulator.simulate2G();
                    simulator.simulate3G();
                    simulator.simulate4G();
                    simulator.simulate5G();
                    break;
                case 0:
                    io.outputstring("\nExiting. Goodbye!\n");
                    io.terminate();
                    running = false;
                    break;
                default:
                    io.outputstring("\nInvalid choice. Please try again.\n");
                    io.terminate();
            }
        }

        return 0;

    } catch (const NetworkException& e) {
        io.errorstring("Fatal Error: ");
        io.errorstring(e.what());
        io.terminate();
        return 1;

    } catch (const std::exception& e) {
        io.errorstring("Standard Exception: ");
        io.errorstring(e.what());
        io.terminate();
        return 1;

    } catch (...) {
        io.errorstring("Unknown error occurred!");
        io.terminate();
        return 1;
    }
}