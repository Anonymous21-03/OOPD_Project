#ifndef CELLULAR_NETWORK_H
#define CELLULAR_NETWORK_H

#include "basicIO.h"
#include <exception>
#include <vector>
#include <memory>

// Custom Exception Classes
class NetworkException : public std::exception {
protected:
    const char* message;
public:
    NetworkException(const char* msg) : message(msg) {}
    const char* what() const noexcept override { return message; }
};

class CapacityExceededException : public NetworkException {
public:
    CapacityExceededException() : NetworkException("Capacity exceeded!") {}
};

class InvalidConfigurationException : public NetworkException {
public:
    InvalidConfigurationException(const char* msg) : NetworkException(msg) {}
};

// Forward declarations
class CellTower;
class CellularCore;

// Enum for Generation Type
enum GenerationType {
    GEN_2G,
    GEN_3G,
    GEN_4G,
    GEN_5G
};

// Template class for managing collections
template<typename T>
class NetworkContainer {
private:
    std::vector<T> items;
public:
    void add(const T& item) { items.push_back(item); }
    T& get(int index) { 
        if (index < 0 || index >= static_cast<int>(items.size())) {
            throw NetworkException("Index out of bounds");
        }
        return items[index]; 
    }
    int size() const { return static_cast<int>(items.size()); }
    void clear() { items.clear(); }

    // Iterator support
    typename std::vector<T>::iterator begin() { return items.begin(); }
    typename std::vector<T>::iterator end() { return items.end(); }
};

// Abstract base class for User Device
class UserDevice {
protected:
    int deviceId;
    int channelId;
    int antennaId;
    bool isActive;

public:
    UserDevice(int id, int channel = 0, int antenna = 0) 
        : deviceId(id), channelId(channel), antennaId(antenna), isActive(true) {}

    virtual ~UserDevice() {}

    // Pure virtual function - polymorphism
    virtual int getMessagesGenerated() const = 0;

    // Getters - data abstraction
    int getDeviceId() const { return deviceId; }
    int getChannelId() const { return channelId; }
    int getAntennaId() const { return antennaId; }
    bool getIsActive() const { return isActive; }

    void setChannelId(int channel) { channelId = channel; }
    void setAntennaId(int antenna) { antennaId = antenna; }
    void deactivate() { isActive = false; }
};

// 2G User Device
class User2G : public UserDevice {
private:
    int dataMessages;
    int voiceMessages;

public:
    User2G(int id, int channel = 0) 
        : UserDevice(id, channel, 0), dataMessages(5), voiceMessages(15) {}

    int getMessagesGenerated() const override {
        return dataMessages + voiceMessages;
    }

    int getDataMessages() const { return dataMessages; }
    int getVoiceMessages() const { return voiceMessages; }
};

// 3G User Device
class User3G : public UserDevice {
private:
    int totalMessages;

public:
    User3G(int id, int channel = 0) 
        : UserDevice(id, channel, 0), totalMessages(10) {}

    int getMessagesGenerated() const override {
        return totalMessages;
    }
};

// 4G User Device
class User4G : public UserDevice {
private:
    int totalMessages;

public:
    User4G(int id, int channel = 0, int antenna = 0) 
        : UserDevice(id, channel, antenna), totalMessages(10) {}

    int getMessagesGenerated() const override {
        return totalMessages;
    }
};

// 5G User Device
class User5G : public UserDevice {
private:
    int totalMessages;
    int frequencyBand; // 0 for original 1MHz, 1 for 1800MHz band

public:
    User5G(int id, int channel = 0, int antenna = 0, int band = 0) 
        : UserDevice(id, channel, antenna), totalMessages(10), frequencyBand(band) {}

    int getMessagesGenerated() const override {
        return totalMessages;
    }

    int getFrequencyBand() const { return frequencyBand; }
};

// Cellular Core class
class CellularCore {
private:
    int coreId;
    int overheadPer100Messages;
    int maxDevicesSupported;

public:
    CellularCore(int id, int overhead) 
        : coreId(id), overheadPer100Messages(overhead), maxDevicesSupported(0) {
        calculateMaxDevices();
    }

    void calculateMaxDevices() {
        // Assuming core can handle a base capacity
        // Overhead reduces effective capacity
        int baseCapacity = 10000;
        maxDevicesSupported = baseCapacity * 100 / (100 + overheadPer100Messages);
    }

    int getCoreId() const { return coreId; }
    int getMaxDevices() const { return maxDevicesSupported; }
    int getOverhead() const { return overheadPer100Messages; }
};

// Cell Tower base class
class CellTower {
protected:
    GenerationType generation;
    int totalBandwidth; // in kHz
    int channelBandwidth; // in kHz
    int usersPerChannel;
    int numAntennas;
    int numChannels;

    NetworkContainer<std::shared_ptr<UserDevice>> users;
    std::vector<CellularCore> cores;

public:
    CellTower(GenerationType gen, int totalBW, int channelBW, int usersPerCh, int antennas)
        : generation(gen), totalBandwidth(totalBW), channelBandwidth(channelBW),
          usersPerChannel(usersPerCh), numAntennas(antennas) {
        numChannels = totalBandwidth / channelBandwidth;
    }

    virtual ~CellTower() {}

    virtual void addUser(std::shared_ptr<UserDevice> user) {
        int totalCapacity = getTotalCapacity();
        if (users.size() >= totalCapacity) {
            throw CapacityExceededException();
        }
        users.add(user);
    }

    virtual int getTotalCapacity() const {
        return numChannels * usersPerChannel * numAntennas;
    }

    virtual int calculateCoresNeeded(int messagesPerUser) const {
        int totalMessages = getTotalCapacity() * messagesPerUser;
        // Assume each core can handle 1000 messages with 10% overhead
        int messagesPerCore = 1000;
        return (totalMessages + messagesPerCore - 1) / messagesPerCore;
    }

    void displayFirstChannelUsers() const;
    void displayTotalCapacity() const;
    void displayCoresNeeded(int messagesPerUser) const;

    int getNumUsers() const { return users.size(); }
    GenerationType getGeneration() const { return generation; }
};

// 2G Tower
class Tower2G : public CellTower {
public:
    Tower2G() : CellTower(GEN_2G, 1000, 200, 16, 1) {}

    void addUser(std::shared_ptr<UserDevice> user) override {
        CellTower::addUser(user);
    }
};

// 3G Tower
class Tower3G : public CellTower {
public:
    Tower3G() : CellTower(GEN_3G, 1000, 200, 32, 1) {}

    void addUser(std::shared_ptr<UserDevice> user) override {
        CellTower::addUser(user);
    }
};

// 4G Tower
class Tower4G : public CellTower {
public:
    Tower4G() : CellTower(GEN_4G, 1000, 10, 30, 4) {}

    void addUser(std::shared_ptr<UserDevice> user) override {
        CellTower::addUser(user);
    }
};

// 5G Tower
class Tower5G : public CellTower {
private:
    int additionalBandwidth; // 10 MHz = 10000 kHz at 1800 MHz
    int usersPerMHz;

public:
    Tower5G() : CellTower(GEN_5G, 1000, 10, 30, 16),
                additionalBandwidth(10000), usersPerMHz(30) {}

    int getTotalCapacity() const override {
        // Original 1 MHz band with 10 kHz channels
        int originalBandCapacity = (totalBandwidth / channelBandwidth) * usersPerChannel * numAntennas;

        // Additional 10 MHz band at 1800 MHz
        int additionalBandCapacity = (additionalBandwidth / 1000) * usersPerMHz * numAntennas;

        return originalBandCapacity + additionalBandCapacity;
    }

    void addUser(std::shared_ptr<UserDevice> user) override {
        CellTower::addUser(user);
    }
};

// Simulation class
class CellularNetworkSimulator {
private:
    std::shared_ptr<CellTower> currentTower;
    GenerationType currentGeneration;

public:
    CellularNetworkSimulator() : currentTower(nullptr), currentGeneration(GEN_2G) {}

    void simulate2G();
    void simulate3G();
    void simulate4G();
    void simulate5G();

    void runSimulation();
};

#endif // CELLULAR_NETWORK_H
