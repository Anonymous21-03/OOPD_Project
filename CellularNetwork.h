#ifndef CELLULAR_NETWORK_H
#define CELLULAR_NETWORK_H

#include "basicIO.h"
#include <memory>
#include <vector>
#include <stdexcept>

// ============================================================================
// CUSTOM EXCEPTION CLASSES - Exception Handling Requirement
// ============================================================================

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

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

class CellTower;
class CellularCore;

// ============================================================================
// ENUM FOR GENERATION TYPE
// ============================================================================

enum GenerationType {
    GEN_2G,
    GEN_3G,
    GEN_4G,
    GEN_5G
};

// ============================================================================
// TEMPLATE CLASS - Template Requirement
// ============================================================================

template <typename T>
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
    
    typename std::vector<T>::iterator begin() { return items.begin(); }
    typename std::vector<T>::iterator end() { return items.end(); }
};

// ============================================================================
// USER DEVICE HIERARCHY - Inheritance & Polymorphism
// ============================================================================

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
    
    virtual int getMessagesGenerated() const = 0;
    
    int getDeviceId() const { return deviceId; }
    int getChannelId() const { return channelId; }
    int getAntennaId() const { return antennaId; }
    bool getIsActive() const { return isActive; }
    
    void setChannelId(int channel) { channelId = channel; }
    void setAntennaId(int antenna) { antennaId = antenna; }
    void deactivate() { isActive = false; }
};

// ============================================================================
// 2G USER DEVICE
// ============================================================================

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

// ============================================================================
// 3G USER DEVICE
// ============================================================================

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

// ============================================================================
// 4G USER DEVICE
// ============================================================================

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

// ============================================================================
// 5G USER DEVICE
// ============================================================================

class User5G : public UserDevice {
private:
    int totalMessages;
    int frequencyBand;
public:
    User5G(int id, int channel = 0, int antenna = 0, int band = 0)
        : UserDevice(id, channel, antenna), totalMessages(10), frequencyBand(band) {}
    
    int getMessagesGenerated() const override {
        return totalMessages;
    }
    
    int getFrequencyBand() const { return frequencyBand; }
};

// ============================================================================
// CELLULAR CORE CLASS
// ============================================================================

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
        int baseCapacity = 10000;
        maxDevicesSupported = baseCapacity * 100 / (100 + overheadPer100Messages);
    }
    
    int getCoreId() const { return coreId; }
    int getMaxDevices() const { return maxDevicesSupported; }
    int getOverhead() const { return overheadPer100Messages; }
};

// ============================================================================
// CELL TOWER BASE CLASS
// ============================================================================

class CellTower {
protected:
    GenerationType generation;
    int totalBandwidth;      // in kHz
    int channelBandwidth;    // in kHz
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
    
    // Declare but don't implement inline - will be implemented in .cpp
    virtual int calculateCoresNeeded(int messagesPerUser) const;
    
    virtual void displayFirstChannelUsers() const;
    void displayTotalCapacity() const;
    void displayCoresNeeded(int messagesPerUser) const;
    
    int getNumUsers() const { return users.size(); }
    GenerationType getGeneration() const { return generation; }

    // New: accessor & setter for antennas so simulation can set antenna count at runtime
    int getNumAntennas() const { return numAntennas; }
    void setNumAntennas(int antennas) { 
        if (antennas < 1) antennas = 1;
        numAntennas = antennas; 
    }

    // Expose users container for iteration in .cpp implementations (kept minimal)
    const NetworkContainer<std::shared_ptr<UserDevice>>& getUsers() const { return users; }
};

 // ============================================================================
// 2G TOWER - TDMA
// ============================================================================

class Tower2G : public CellTower {
public:
    Tower2G() : CellTower(GEN_2G, 1000, 200, 16, 1) {}
    
    void addUser(std::shared_ptr<UserDevice> user) override {
        CellTower::addUser(user);
    }
};

// ============================================================================
// 3G TOWER - CDMA
// ============================================================================

class Tower3G : public CellTower {
public:
    Tower3G() : CellTower(GEN_3G, 1000, 200, 32, 1) {}
    
    void addUser(std::shared_ptr<UserDevice> user) override {
        CellTower::addUser(user);
    }
};

// ============================================================================
// 4G TOWER - OFDM
// ============================================================================

class Tower4G : public CellTower {
public:
    Tower4G() : CellTower(GEN_4G, 1000, 10, 30, 4) {}
    
    void addUser(std::shared_ptr<UserDevice> user) override {
        CellTower::addUser(user);
    }
};

// ============================================================================
// 5G TOWER - Massive MIMO
// ============================================================================

class Tower5G : public CellTower {
private:
    int additionalBandwidth; // in kHz (10 MHz = 10000 kHz)
    int usersPerMHz;
public:
    Tower5G() : CellTower(GEN_5G, 1000, 10, 30, 16),
                additionalBandwidth(10000), usersPerMHz(30) {}
    
    int getTotalCapacity() const override {
        int originalBandCapacity = (totalBandwidth / channelBandwidth) * usersPerChannel * numAntennas;
        int additionalBandCapacity = (additionalBandwidth / 1000) * usersPerMHz * numAntennas;
        return originalBandCapacity + additionalBandCapacity;
    }
    
    void addUser(std::shared_ptr<UserDevice> user) override {
        CellTower::addUser(user);
    }
    
    // FIXED: Override to filter by frequency band as well
    void displayFirstChannelUsers() const override;
};

 // ============================================================================
// CELLULAR NETWORK SIMULATOR
// ============================================================================

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
    void runSimulation(); // original "run all" method
};

#endif // CELLULAR_NETWORK_H