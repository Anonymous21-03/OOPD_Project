# ============================================================================
# Makefile - Cellular Network Simulator
# OOPD Project - Monsoon 2025
# ============================================================================
# This Makefile builds both debug and release versions of the simulator
# ============================================================================

# Compiler and flags
CXX = g++
AS = as
CXXFLAGS = -std=c++17 -Wall -Wextra

# Debug flags
DEBUG_FLAGS = -g -O0 -DDEBUG
DEBUG_CXXFLAGS = $(CXXFLAGS) $(DEBUG_FLAGS)

# Release flags (optimized)
RELEASE_FLAGS = -O3 -DNDEBUG
RELEASE_CXXFLAGS = $(CXXFLAGS) $(RELEASE_FLAGS)

# Source files
SOURCES = main.cpp basicIO.cpp CellularNetwork.cpp
HEADERS = basicIO.h CellularNetwork.h
ASMSOURCES = syscall.s

# Object files - separated by build type
DEBUG_OBJECTS = $(addprefix debug_obj/,$(SOURCES:.cpp=.o)) debug_obj/syscall.o
RELEASE_OBJECTS = $(addprefix release_obj/,$(SOURCES:.cpp=.o)) release_obj/syscall.o

# Output binaries
DEBUG_BIN = cellular_network_debug
RELEASE_BIN = cellular_network

# Default target
all: debug release

# ============================================================================
# DEBUG BUILD TARGETS
# ============================================================================

debug: $(DEBUG_BIN)

$(DEBUG_BIN): $(DEBUG_OBJECTS)
	@echo "Linking debug version: $(DEBUG_BIN)"
	@$(CXX) $(DEBUG_CXXFLAGS) -o $@ $^
	@echo "✓ Debug build complete: ./$(DEBUG_BIN)"

debug_obj/%.o: %.cpp $(HEADERS)
	@mkdir -p debug_obj
	@echo "Compiling [DEBUG]: $<"
	@$(CXX) $(DEBUG_CXXFLAGS) -c $< -o $@

debug_obj/syscall.o: syscall.s
	@mkdir -p debug_obj
	@echo "Assembling [DEBUG]: syscall.s"
	@$(AS) -o $@ $<

# ============================================================================
# RELEASE BUILD TARGETS
# ============================================================================

release: $(RELEASE_BIN)

$(RELEASE_BIN): $(RELEASE_OBJECTS)
	@echo "Linking release version: $(RELEASE_BIN)"
	@$(CXX) $(RELEASE_CXXFLAGS) -o $@ $^
	@echo "✓ Release build complete: ./$(RELEASE_BIN)"

release_obj/%.o: %.cpp $(HEADERS)
	@mkdir -p release_obj
	@echo "Compiling [RELEASE]: $<"
	@$(CXX) $(RELEASE_CXXFLAGS) -c $< -o $@

release_obj/syscall.o: syscall.s
	@mkdir -p release_obj
	@echo "Assembling [RELEASE]: syscall.s"
	@$(AS) -o $@ $<

# ============================================================================
# UTILITY TARGETS
# ============================================================================

run: release
	@echo "Running release version..."
	@./$(RELEASE_BIN)

run-debug: debug
	@echo "Running debug version..."
	@./$(DEBUG_BIN)

clean:
	@echo "Cleaning build artifacts..."
	@rm -rf debug_obj/ release_obj/
	@rm -f $(DEBUG_BIN) $(RELEASE_BIN)
	@echo "✓ Cleanup complete"

rebuild: clean all
	@echo "✓ Rebuild complete"

.PHONY: all debug release run run-debug clean rebuild

# ============================================================================
# HELP TARGET
# ============================================================================

help:
	@echo "===== Cellular Network Simulator - Build System ====="
	@echo ""
	@echo "Available targets:"
	@echo "  make all          - Build both debug and release versions (default)"
	@echo "  make debug        - Build debug version only"
	@echo "  make release      - Build release version only"
	@echo "  make run          - Build and run release version"
	@echo "  make run-debug    - Build and run debug version"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make rebuild      - Clean and rebuild all"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Output binaries:"
	@echo "  Debug:   ./$(DEBUG_BIN)"
	@echo "  Release: ./$(RELEASE_BIN)"
	@echo ""
