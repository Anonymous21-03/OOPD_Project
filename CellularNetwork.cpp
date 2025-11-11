#include "CellularNetwork.h"
#include "basicIO.h"

// ============================================================================
// CELL TOWER IMPLEMENTATION
// ============================================================================

void CellTower::displayFirstChannelUsers() const {
    io.outputstring("Users on first channel: ");
    int firstChannelCount = 0;
    
    for (int i = 0; i < const_cast<CellTower*>(this)->users.size(); i++) {
        std::shared_ptr<UserDevice> user = const_cast<CellTower*>(this)->users.get(i);
        // FIXED: Check BOTH channel AND antenna
        if (user->getChannelId() == 0 && user->getAntennaId() == 0) {
            if (firstChannelCount > 0) {
                io.outputstring(", ");
            }
            io.outputint(user->getDeviceId());
            firstChannelCount++;
        }
    }
    
    if (firstChannelCount == 0) {
        io.outputstring("None");
    }
    io.terminate();
}

void CellTower::displayTotalCapacity() const {
    io.outputstring("Total capacity: ");
    io.outputint(getTotalCapacity());
    io.outputstring(" users");
    io.terminate();
}

void CellTower::displayCoresNeeded(int messagesPerUser) const {
    io.outputstring("Cellular cores needed: ");
    io.outputint(calculateCoresNeeded(messagesPerUser));
    io.terminate();
}

// ============================================================================
// CELL TOWER - Calculate cores needed method (IMPLEMENTATION)
// ============================================================================

int CellTower::calculateCoresNeeded(int messagesPerUser) const {
    int totalMessages = getTotalCapacity() * messagesPerUser;
    int messagesPerCore = 100;  // FIXED: Changed from 1000 to 100
    return (totalMessages + messagesPerCore - 1) / messagesPerCore;
}

// ============================================================================
// 2G SIMULATION - TDMA (Time Division Multiple Access)
// Requirement 2.1-2.6
// ============================================================================

void CellularNetworkSimulator::simulate2G() {
    io.outputstring("\n========== 2G COMMUNICATION SIMULATION ==========");
    io.terminate();
    
    try {
        currentTower = std::make_shared<Tower2G>();
        currentGeneration = GEN_2G;
        
        int totalCapacity = currentTower->getTotalCapacity();
        
        io.outputstring("Technology: TDMA (Time Division Multiple Access)");
        io.terminate();
        io.outputstring("Bandwidth: 1 MHz (1000 kHz)");
        io.terminate();
        io.outputstring("Channel bandwidth: 200 kHz");
        io.terminate();
        io.outputstring("Number of channels: ");
        io.outputint(1000 / 200);
        io.terminate();
        io.outputstring("Users per channel: 16");
        io.terminate();
        io.outputstring("Messages per user: 20 (5 data + 15 voice)");
        io.terminate();
        
        currentTower->displayTotalCapacity();
        
        io.outputstring("\nAdding users to first channel (0-200 kHz)...");
        io.terminate();
        
        int usersInFirstChannel = 16;
        for (int i = 0; i < usersInFirstChannel; i++) {
            auto user = std::make_shared<User2G>(i, 0);
            currentTower->addUser(user);
        }
        
        int channelId = 1;
        int usersInChannel = 0;
        for (int i = usersInFirstChannel; i < totalCapacity; i++) {
            auto user = std::make_shared<User2G>(i, channelId);
            currentTower->addUser(user);
            usersInChannel++;
            if (usersInChannel >= 16) {
                channelId++;
                usersInChannel = 0;
            }
        }
        
        currentTower->displayFirstChannelUsers();
        currentTower->displayCoresNeeded(20);
        
    } catch (const NetworkException& e) {
        io.errorstring("2G Simulation Error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

// ============================================================================
// 3G SIMULATION - CDMA (Code Division Multiple Access)
// Requirement 3.1-3.6
// ============================================================================

void CellularNetworkSimulator::simulate3G() {
    io.outputstring("\n========== 3G COMMUNICATION SIMULATION ==========");
    io.terminate();
    
    try {
        currentTower = std::make_shared<Tower3G>();
        currentGeneration = GEN_3G;
        
        int totalCapacity = currentTower->getTotalCapacity();
        
        io.outputstring("Technology: CDMA (Code Division Multiple Access)");
        io.terminate();
        io.outputstring("Bandwidth: 1 MHz (1000 kHz)");
        io.terminate();
        io.outputstring("Channel bandwidth: 200 kHz");
        io.terminate();
        io.outputstring("Number of channels: ");
        io.outputint(1000 / 200);
        io.terminate();
        io.outputstring("Users per channel: 32");
        io.terminate();
        io.outputstring("Messages per user: 10");
        io.terminate();
        
        currentTower->displayTotalCapacity();
        
        io.outputstring("\nAdding users to first channel (0-200 kHz)...");
        io.terminate();
        
        int usersInFirstChannel = 32;
        for (int i = 0; i < usersInFirstChannel; i++) {
            auto user = std::make_shared<User3G>(i, 0);
            currentTower->addUser(user);
        }
        
        int channelId = 1;
        int usersInChannel = 0;
        for (int i = usersInFirstChannel; i < totalCapacity; i++) {
            auto user = std::make_shared<User3G>(i, channelId);
            currentTower->addUser(user);
            usersInChannel++;
            if (usersInChannel >= 32) {
                channelId++;
                usersInChannel = 0;
            }
        }
        
        currentTower->displayFirstChannelUsers();
        currentTower->displayCoresNeeded(10);
        
    } catch (const NetworkException& e) {
        io.errorstring("3G Simulation Error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

// ============================================================================
// 4G SIMULATION - OFDM (Orthogonal Frequency Division Multiplexing)
// Requirement 4.1-4.7
// ============================================================================

void CellularNetworkSimulator::simulate4G() {
    io.outputstring("\n========== 4G COMMUNICATION SIMULATION ==========");
    io.terminate();
    
    try {
        currentTower = std::make_shared<Tower4G>();
        currentGeneration = GEN_4G;
        
        int totalCapacity = currentTower->getTotalCapacity();
        
        io.outputstring("Technology: OFDM (Orthogonal Frequency Division Multiplexing)");
        io.terminate();
        io.outputstring("Bandwidth: 1 MHz (1000 kHz)");
        io.terminate();
        io.outputstring("Channel bandwidth: 10 kHz");
        io.terminate();
        io.outputstring("Number of channels: ");
        io.outputint(1000 / 10);
        io.terminate();
        io.outputstring("Users per channel: 30");
        io.terminate();
        io.outputstring("Number of antennas: 4");
        io.terminate();
        io.outputstring("Messages per user: 10");
        io.terminate();
        
        currentTower->displayTotalCapacity();
        
        io.outputstring("\nAdding users to first channel (0-10 kHz, Antenna 0)...");
        io.terminate();
        
        // FIXED: Only add to channel 0, antenna 0 (not all antennas)
        int usersInFirstChannel = 30;
        for (int i = 0; i < usersInFirstChannel; i++) {
            auto user = std::make_shared<User4G>(i, 0, 0);
            currentTower->addUser(user);
        }
        
        int userId = usersInFirstChannel;
        
        // Add remaining users starting from channel 1 for antenna 0
        for (int channel = 1; channel < 100; channel++) {
            for (int u = 0; u < 30; u++) {
                if (userId < totalCapacity) {
                    auto user = std::make_shared<User4G>(userId++, channel, 0);
                    currentTower->addUser(user);
                }
            }
        }
        
        // Add users for antennas 1, 2, 3 (starting from channel 0)
        for (int antenna = 1; antenna < 4; antenna++) {
            for (int channel = 0; channel < 100; channel++) {
                for (int u = 0; u < 30; u++) {
                    if (userId < totalCapacity) {
                        auto user = std::make_shared<User4G>(userId++, channel, antenna);
                        currentTower->addUser(user);
                    }
                }
            }
        }
        
        currentTower->displayFirstChannelUsers();
        currentTower->displayCoresNeeded(10);
        
    } catch (const NetworkException& e) {
        io.errorstring("4G Simulation Error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

// ============================================================================
// 5G SIMULATION - Massive MIMO + OFDM
// Requirement 5.1-5.7
// ============================================================================

void CellularNetworkSimulator::simulate5G() {
    io.outputstring("\n========== 5G COMMUNICATION SIMULATION ==========");
    io.terminate();
    
    try {
        currentTower = std::make_shared<Tower5G>();
        currentGeneration = GEN_5G;
        
        int totalCapacity = currentTower->getTotalCapacity();
        
        io.outputstring("Technology: Massive MIMO + OFDM");
        io.terminate();
        io.outputstring("Primary bandwidth: 1 MHz (1000 kHz)");
        io.terminate();
        io.outputstring("Additional bandwidth: 10 MHz at 1800 MHz");
        io.terminate();
        io.outputstring("Channel bandwidth (primary): 10 kHz");
        io.terminate();
        io.outputstring("Users per 1 MHz (1800 MHz band): 30");
        io.terminate();
        io.outputstring("Number of antennas: 16");
        io.terminate();
        io.outputstring("Messages per user: 10");
        io.terminate();
        
        currentTower->displayTotalCapacity();
        
        io.outputstring("\nAdding users to first channel (0-10 kHz, Antenna 0, Primary band)...");
        io.terminate();
        
        // FIXED: Only add to channel 0, antenna 0, band 0
        int usersInFirstChannel = 30;
        for (int i = 0; i < usersInFirstChannel; i++) {
            auto user = std::make_shared<User5G>(i, 0, 0, 0);
            currentTower->addUser(user);
        }
        
        int userId = usersInFirstChannel;
        
        // Add remaining users for primary band (1 MHz) on antenna 0
        for (int channel = 1; channel < 100; channel++) {
            for (int u = 0; u < 30; u++) {
                if (userId < totalCapacity) {
                    auto user = std::make_shared<User5G>(userId++, channel, 0, 0);
                    currentTower->addUser(user);
                }
            }
        }
        
        // Add users for antennas 1-15 on primary band
        for (int antenna = 1; antenna < 16; antenna++) {
            for (int channel = 0; channel < 100; channel++) {
                for (int u = 0; u < 30; u++) {
                    if (userId < totalCapacity) {
                        auto user = std::make_shared<User5G>(userId++, channel, antenna, 0);
                        currentTower->addUser(user);
                    }
                }
            }
        }
        
        // Add users for additional band (10 MHz at 1800 MHz)
        for (int antenna = 0; antenna < 16; antenna++) {
            for (int mhz_channel = 0; mhz_channel < 10; mhz_channel++) {
                for (int u = 0; u < 30; u++) {
                    if (userId < totalCapacity) {
                        auto user = std::make_shared<User5G>(userId++, mhz_channel, antenna, 1);
                        currentTower->addUser(user);
                    }
                }
            }
        }
        
        currentTower->displayFirstChannelUsers();
        currentTower->displayCoresNeeded(10);
        
    } catch (const NetworkException& e) {
        io.errorstring("5G Simulation Error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

// ============================================================================
// MAIN SIMULATION RUNNER
// ============================================================================

void CellularNetworkSimulator::runSimulation() {
    io.outputstring("=================================================");
    io.terminate();
    io.outputstring(" CELLULAR NETWORK SIMULATOR");
    io.terminate();
    io.outputstring(" OOPD Project - Monsoon 2025");
    io.terminate();
    io.outputstring("=================================================");
    io.terminate();
    
    simulate2G();
    simulate3G();
    simulate4G();
    simulate5G();
    
    io.outputstring("\n=================================================");
    io.terminate();
    io.outputstring(" SIMULATION COMPLETE");
    io.terminate();
    io.outputstring("=================================================");
    io.terminate();
}
