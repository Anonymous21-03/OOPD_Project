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
        // Check BOTH channel AND antenna
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
// FIXED: Use messagesPerCore = 1000 to match expected output
// ============================================================================

int CellTower::calculateCoresNeeded(int messagesPerUser) const {
    int totalMessages = getTotalCapacity() * messagesPerUser;
    int messagesPerCore = 1000;  // per spec / expected output
    return (totalMessages + messagesPerCore - 1) / messagesPerCore;
}

// ============================================================================
// 5G TOWER - Override to display first channel correctly (accounts for band)
// ============================================================================

void Tower5G::displayFirstChannelUsers() const {
    io.outputstring("Users on first channel: ");
    int firstChannelCount = 0;
    
    for (int i = 0; i < const_cast<Tower5G*>(this)->users.size(); i++) {
        std::shared_ptr<UserDevice> user = const_cast<Tower5G*>(this)->users.get(i);
        User5G* user5g = dynamic_cast<User5G*>(user.get());
        if (user->getChannelId() == 0 && user->getAntennaId() == 0 && 
            user5g && user5g->getFrequencyBand() == 0) {
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

// ============================================================================
// 2G SIMULATION - TDMA (Time Division Multiple Access)
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
// ============================================================================

void CellularNetworkSimulator::simulate4G() {
    io.outputstring("\n========== 4G COMMUNICATION SIMULATION ==========");
    io.terminate();
    
    try {
        // create tower (default antennas = 4)
        currentTower = std::make_shared<Tower4G>();
        currentGeneration = GEN_4G;
        
        // Prompt user for number of antennas (range: 1..4) as spec allows "up to 4"
        io.outputstring("Enter number of antennas for 4G (1-4) [default 4]: ");
        char buf[32] = {0};
        io.inputstring(buf, 32);
        int antennas = atoi(buf);
        if (antennas < 1 || antennas > 4) antennas = 4;
        currentTower->setNumAntennas(antennas);
        
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
        io.outputstring("Number of antennas: ");
        io.outputint(currentTower->getNumAntennas());
        io.terminate();
        io.outputstring("Messages per user: 10");
        io.terminate();
        
        currentTower->displayTotalCapacity();
        
        io.outputstring("\nAdding users to first channel (0-10 kHz, Antenna 0)...");
        io.terminate();
        
        int usersInFirstChannel = 30;
        for (int i = 0; i < usersInFirstChannel; i++) {
            auto user = std::make_shared<User4G>(i, 0, 0);
            currentTower->addUser(user);
        }
        
        int userId = usersInFirstChannel;
        
        for (int channel = 1; channel < 100; channel++) {
            for (int u = 0; u < 30; u++) {
                if (userId < totalCapacity) {
                    auto user = std::make_shared<User4G>(userId++, channel, 0);
                    currentTower->addUser(user);
                }
            }
        }
        
        for (int antenna = 1; antenna < currentTower->getNumAntennas(); antenna++) {
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
// ============================================================================

void CellularNetworkSimulator::simulate5G() {
    io.outputstring("\n========== 5G COMMUNICATION SIMULATION ==========");
    io.terminate();
    
    try {
        // create tower (default antennas = 16)
        currentTower = std::make_shared<Tower5G>();
        currentGeneration = GEN_5G;
        
        // Prompt user for number of antennas (range: 1..16) as spec allows "up to 16"
        io.outputstring("Enter number of antennas for 5G (1-16) [default 16]: ");
        char buf[32] = {0};
        io.inputstring(buf, 32);
        int antennas = atoi(buf);
        if (antennas < 1 || antennas > 16) antennas = 16;
        currentTower->setNumAntennas(antennas);
        
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
        io.outputstring("Number of antennas: ");
        io.outputint(currentTower->getNumAntennas());
        io.terminate();
        io.outputstring("Messages per user: 10");
        io.terminate();
        
        currentTower->displayTotalCapacity();
        
        io.outputstring("\nAdding users to first channel (0-10 kHz, Antenna 0, Primary band)...");
        io.terminate();
        
        int usersInFirstChannel = 30;
        for (int i = 0; i < usersInFirstChannel; i++) {
            auto user = std::make_shared<User5G>(i, 0, 0, 0);
            currentTower->addUser(user);
        }
        
        int userId = usersInFirstChannel;
        
        for (int channel = 1; channel < 100; channel++) {
            for (int u = 0; u < 30; u++) {
                if (userId < totalCapacity) {
                    auto user = std::make_shared<User5G>(userId++, channel, 0, 0);
                    currentTower->addUser(user);
                }
            }
        }
        
        for (int antenna = 1; antenna < currentTower->getNumAntennas(); antenna++) {
            for (int channel = 0; channel < 100; channel++) {
                for (int u = 0; u < 30; u++) {
                    if (userId < totalCapacity) {
                        auto user = std::make_shared<User5G>(userId++, channel, antenna, 0);
                        currentTower->addUser(user);
                    }
                }
            }
        }
        
        for (int antenna = 0; antenna < currentTower->getNumAntennas(); antenna++) {
            for (int mhz_channel = 0; mhz_channel < 10; mhz_channel++) {
                for (int u = 0; u < 30; u++) {
                    if (userId < totalCapacity) {
                        auto user = std::make_shared<User5G>(userId++, mhz_channel, antenna, 1);
                        currentTower->addUser(user);
                    }
                }
            }
        }
        
        // Use Tower5G's specialized displayFirstChannelUsers
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