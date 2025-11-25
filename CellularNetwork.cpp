// CellularNetwork.cpp
#include "CellularNetwork.h"
#include "basicIO.h"
#include <cstdlib>
#include <cstring>

// ============================================================================
// GLOBAL I/O (basicIO implemented in basicIO.cpp)
extern basicIO io;

// ============================================================================
// CellTower — default implementations for displays & cores calculation
// ============================================================================

void CellTower::displayFirstChannelUsers() const {
    io.outputstring("Users on first channel: ");
    int count = 0;
    const auto& container = getUsers();
    for (int i = 0; i < container.size(); ++i) {
        const auto& user = container.get(i);
        if (!user) continue;
        if (user->getChannelId() == 0 && user->getAntennaId() == 0) {
            if (count > 0) io.outputstring(", ");
            io.outputint(user->getDeviceId());
            ++count;
        }
    }
    if (count == 0) io.outputstring("None");
    io.terminate();
}

void CellTower::displayTotalCapacity() const {
    io.outputstring("Total capacity: ");
    io.outputint(getTotalCapacity());
    io.outputstring(" users");
    io.terminate();
}

void CellTower::displayCoresNeeded(int messagesPerUser, int overheadPer100Messages) const {
    io.outputstring("Cellular cores needed: ");
    io.outputint(calculateCoresNeeded(messagesPerUser, overheadPer100Messages));
    io.terminate();
}

int CellTower::calculateCoresNeeded(int messagesPerUser, int overheadPer100Messages) const {
    // Correct calculation: cores are determined by total messages generated
    // and the core capacity in messages (which is reduced by overhead).
    CellularCore sampleCore(0, overheadPer100Messages);
    int maxMessagesPerCore = sampleCore.getMaxMessages();
    if (maxMessagesPerCore <= 0) {
        // fallback to a safe non-zero
        maxMessagesPerCore = 1;
    }

    long long totalDevices = static_cast<long long>(getTotalCapacity());
    long long totalMessages = totalDevices * static_cast<long long>(messagesPerUser);

    long long cores = (totalMessages + (long long)maxMessagesPerCore - 1) / (long long)maxMessagesPerCore;
    if (cores < 1) cores = 1;
    // safe cast back to int (reasonable assumption for project sizes)
    return static_cast<int>(cores);
}

// ============================================================================
// Tower5G - specialized first-channel display (filters by frequency band)
// ============================================================================

void Tower5G::displayFirstChannelUsers() const {
    io.outputstring("Users on first channel: ");
    int count = 0;
    const auto& container = getUsers();
    for (int i = 0; i < container.size(); ++i) {
        const auto& userPtr = container.get(i);
        if (!userPtr) continue;
        // Only show users that are in channel 0, antenna 0, and primary band (band == 0)
        if (userPtr->getChannelId() == 0 && userPtr->getAntennaId() == 0) {
            User5G* u5 = dynamic_cast<User5G*>(userPtr.get());
            if (u5 && u5->getFrequencyBand() == 0) {
                if (count > 0) io.outputstring(", ");
                io.outputint(u5->getDeviceId());
                ++count;
            }
        }
    }
    if (count == 0) io.outputstring("None");
    io.terminate();
}

// ============================================================================
// 2G Simulation
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
        for (int i = 0; i < usersInFirstChannel; ++i) {
            auto user = std::make_shared<User2G>(i, 0);
            currentTower->addUser(user);
        }

        int channelId = 1;
        int usersInChannel = 0;
        for (int i = usersInFirstChannel; i < totalCapacity; ++i) {
            auto user = std::make_shared<User2G>(i, channelId);
            currentTower->addUser(user);
            usersInChannel++;
            if (usersInChannel >= 16) {
                channelId++;
                usersInChannel = 0;
            }
        }

        currentTower->displayFirstChannelUsers();

        // prompt for overhead per 100 messages (default 0)
        io.outputstring("\nEnter overhead per 100 messages (0-100) [default 0]: ");
        char buf[32] = {0};
        io.inputstring(buf, 32);
        int overhead = atoi(buf);
        if (buf[0] == '\0') overhead = 0;
        if (overhead < 0) overhead = 0;

        currentTower->displayCoresNeeded(20, overhead);

    } catch (const NetworkException& e) {
        io.errorstring("2G Simulation Error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

// ============================================================================
// 3G Simulation
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
        for (int i = 0; i < usersInFirstChannel; ++i) {
            auto user = std::make_shared<User3G>(i, 0);
            currentTower->addUser(user);
        }

        int channelId = 1;
        int usersInChannel = 0;
        for (int i = usersInFirstChannel; i < totalCapacity; ++i) {
            auto user = std::make_shared<User3G>(i, channelId);
            currentTower->addUser(user);
            usersInChannel++;
            if (usersInChannel >= 32) {
                channelId++;
                usersInChannel = 0;
            }
        }

        currentTower->displayFirstChannelUsers();

        io.outputstring("\nEnter overhead per 100 messages (0-100) [default 0]: ");
        char buf[32] = {0};
        io.inputstring(buf, 32);
        int overhead = atoi(buf);
        if (buf[0] == '\0') overhead = 0;
        if (overhead < 0) overhead = 0;

        currentTower->displayCoresNeeded(10, overhead);

    } catch (const NetworkException& e) {
        io.errorstring("3G Simulation Error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

// ============================================================================
// 4G Simulation
// ============================================================================
void CellularNetworkSimulator::simulate4G() {
    io.outputstring("\n========== 4G COMMUNICATION SIMULATION ==========");
    io.terminate();

    try {
        currentTower = std::make_shared<Tower4G>();
        currentGeneration = GEN_4G;

        // prompt and set antennas (1..4)
        io.outputstring("Enter number of antennas for 4G (1-4) [default 4]: ");
        char buf[32] = {0};
        io.inputstring(buf, 32);
        int antennas = atoi(buf);
        if (buf[0] == '\0') antennas = 4;
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
        for (int i = 0; i < usersInFirstChannel; ++i) {
            auto user = std::make_shared<User4G>(i, 0, 0);
            currentTower->addUser(user);
        }

        int userId = usersInFirstChannel;

        // fill remaining channels for antenna 0
        for (int channel = 1; channel < 100; ++channel) {
            for (int u = 0; u < 30; ++u) {
                if (userId < totalCapacity) {
                    auto user = std::make_shared<User4G>(userId++, channel, 0);
                    currentTower->addUser(user);
                }
            }
        }

        // fill other antenna layers (reuse)
        for (int antenna = 1; antenna < currentTower->getNumAntennas(); ++antenna) {
            for (int channel = 0; channel < 100; ++channel) {
                for (int u = 0; u < 30; ++u) {
                    if (userId < totalCapacity) {
                        auto user = std::make_shared<User4G>(userId++, channel, antenna);
                        currentTower->addUser(user);
                    }
                }
            }
        }

        currentTower->displayFirstChannelUsers();

        io.outputstring("\nEnter overhead per 100 messages (0-100) [default 0]: ");
        char buf2[32] = {0};
        io.inputstring(buf2, 32);
        int overhead = atoi(buf2);
        if (buf2[0] == '\0') overhead = 0;
        if (overhead < 0) overhead = 0;

        currentTower->displayCoresNeeded(10, overhead);

    } catch (const NetworkException& e) {
        io.errorstring("4G Simulation Error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

// ============================================================================
// 5G Simulation
// ============================================================================
void CellularNetworkSimulator::simulate5G() {
    io.outputstring("\n========== 5G COMMUNICATION SIMULATION ==========");
    io.terminate();

    try {
        currentTower = std::make_shared<Tower5G>();
        currentGeneration = GEN_5G;

        // prompt antennas (1..16)
        io.outputstring("Enter number of antennas for 5G (1-16) [default 16]: ");
        char buf[32] = {0};
        io.inputstring(buf, 32);
        int antennas = atoi(buf);
        if (buf[0] == '\0') antennas = 16;
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
        for (int i = 0; i < usersInFirstChannel; ++i) {
            auto user = std::make_shared<User5G>(i, 0, 0, 0); // band=0 primary
            currentTower->addUser(user);
        }

        int userId = usersInFirstChannel;

        // fill remaining primary-band channels (antenna 0)
        for (int channel = 1; channel < 100; ++channel) {
            for (int u = 0; u < 30; ++u) {
                if (userId < totalCapacity) {
                    auto user = std::make_shared<User5G>(userId++, channel, 0, 0);
                    currentTower->addUser(user);
                }
            }
        }

        // fill primary-band across other antennas (reuse)
        for (int antenna = 1; antenna < currentTower->getNumAntennas(); ++antenna) {
            for (int channel = 0; channel < 100; ++channel) {
                for (int u = 0; u < 30; ++u) {
                    if (userId < totalCapacity) {
                        auto user = std::make_shared<User5G>(userId++, channel, antenna, 0);
                        currentTower->addUser(user);
                    }
                }
            }
        }

        // fill the additional 10 MHz@1800MHz band: we treat each MHz as a "channel group"
        for (int antenna = 0; antenna < currentTower->getNumAntennas(); ++antenna) {
            for (int mhz_channel = 0; mhz_channel < 10; ++mhz_channel) {
                for (int u = 0; u < 30; ++u) {
                    if (userId < totalCapacity) {
                        auto user = std::make_shared<User5G>(userId++, mhz_channel, antenna, 1); // band=1
                        currentTower->addUser(user);
                    }
                }
            }
        }

        currentTower->displayFirstChannelUsers();

        io.outputstring("\nEnter overhead per 100 messages (0-100) [default 0]: ");
        char buf3[32] = {0};
        io.inputstring(buf3, 32);
        int overhead = atoi(buf3);
        if (buf3[0] == '\0') overhead = 0;
        if (overhead < 0) overhead = 0;

        currentTower->displayCoresNeeded(10, overhead);

    } catch (const NetworkException& e) {
        io.errorstring("5G Simulation Error: ");
        io.errorstring(e.what());
        io.terminate();
    }
}

// ============================================================================
// runSimulation (used when you want to run all sims sequentially)
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
