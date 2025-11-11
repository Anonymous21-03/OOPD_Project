#include "CellularNetwork.h"
#include "basicIO.h"

int main() {
    try {
        // Create and run the cellular network simulator
        CellularNetworkSimulator simulator;
        simulator.runSimulation();
        
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