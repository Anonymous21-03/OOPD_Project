# Makefile for Cellular Network Simulator
# OOPD Project - Monsoon 2025

CXX = g++
CXXFLAGS_COMMON = -std=c++17 -Wall -Wextra
CXXFLAGS_DEBUG = $(CXXFLAGS_COMMON) -g -O0 -DDEBUG
CXXFLAGS_RELEASE = $(CXXFLAGS_COMMON) -O3 -DNDEBUG

AS = as
ASFLAGS = 

LDFLAGS = 

# Source files
SOURCES = main.cpp CellularNetwork.cpp basicIO.cpp
ASM_SOURCES = syscall.s

# Object files
OBJECTS_DEBUG = $(SOURCES:.cpp=_debug.o) $(ASM_SOURCES:.s=_debug.o)
OBJECTS_RELEASE = $(SOURCES:.cpp=_release.o) $(ASM_SOURCES:.s=_release.o)

# Output binaries
TARGET_DEBUG = cellular_network_debug
TARGET_RELEASE = cellular_network

# Default target
all: debug release

# Debug build
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(OBJECTS_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) -o $@ $^ $(LDFLAGS)

%_debug.o: %.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@

%_debug.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Release/Optimized build
release: $(TARGET_RELEASE)

$(TARGET_RELEASE): $(OBJECTS_RELEASE)
	$(CXX) $(CXXFLAGS_RELEASE) -o $@ $^ $(LDFLAGS)

%_release.o: %.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -c $< -o $@

%_release.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS_DEBUG) $(OBJECTS_RELEASE) $(TARGET_DEBUG) $(TARGET_RELEASE)
	rm -f *_debug.o *_release.o

# Run debug version
run-debug: debug
	./$(TARGET_DEBUG)

# Run release version
run: release
	./$(TARGET_RELEASE)

# Help target
help:
	@echo "Cellular Network Simulator - Build System"
	@echo "=========================================="
	@echo "Available targets:"
	@echo "  all         - Build both debug and release versions (default)"
	@echo "  debug       - Build debug version with symbols"
	@echo "  release     - Build optimized release version"
	@echo "  clean       - Remove all build artifacts"
	@echo "  run-debug   - Build and run debug version"
	@echo "  run         - Build and run release version"
	@echo "  help        - Display this help message"

.PHONY: all debug release clean run run-debug help
