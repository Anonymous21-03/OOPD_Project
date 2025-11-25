// main.cpp
#include "CellularNetwork.h"
#include "basicIO.h"
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <iostream>

extern basicIO io;

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

// Redirect stdin (fd 0) to the named file.
// Returns true on success, false on failure (and prints an error message to stderr).
static bool redirectStdinToFile(const char* path) {
    if (!path) return false;
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Error: cannot open input file \"" << path << "\": " << strerror(errno) << std::endl;
        return false;
    }
    if (dup2(fd, STDIN_FILENO) < 0) {
        std::cerr << "Error: dup2 failed for \"" << path << "\": " << strerror(errno) << std::endl;
        close(fd);
        return false;
    }
    close(fd);
    return true;
}

int main(int argc, char** argv) {
    try {
        // If filename provided as first argument, redirect stdin to it.
        // This makes all existing calls that read from STDIN (via basicIO/syscall) work unchanged.
        if (argc > 1) {
            if (!redirectStdinToFile(argv[1])) {
                std::cerr << "Falling back to interactive stdin is not attempted; exiting." << std::endl;
                return 1;
            }
        }

        CellularNetworkSimulator simulator;
        bool running = true;
        printHeader();

        while (running) {
            printMenu();
            char buf[256];
            // read menu choice (works from redirected file or terminal)
            io.inputstring(buf, 256);

            int choice = atoi(buf);

            // If atoi gives 0 but input wasn't actually "0", treat as invalid
            if (choice == 0 && buf[0] != '0' && buf[0] != '\n' && buf[0] != '\0')
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
                    // simulate all sequentially (these will consume further lines from the input file
                    // for any interactive prompts inside each simulation)
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
