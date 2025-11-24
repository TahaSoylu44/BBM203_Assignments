#include "QuakeAssistController.h"
#include <iostream>
#include <fstream>
#include <string>

// This main file is provided for you.
// It reads commands from the given input file and forwards them
// to the QuakeAssistController. You do NOT need to modify main.cpp.

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: QuakeAssist <input_file>" << std::endl;
        return 1;
    }

    QuakeAssistController controller;

    // The number of teams will be determined from the input file
    // using the INIT_TEAMS command (and then SET_TEAM_CAPACITY).
    // Do NOT call initializeTeams() here for a fixed number.

    std::ifstream in(argv[1]);
    if (!in.is_open()) {
        std::cerr << "Could not open input file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(in, line)) {
        bool cont = controller.parseAndExecute(line);
        if (!cont) break;
    }

    return 0;
}
