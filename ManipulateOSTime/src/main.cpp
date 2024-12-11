#include "TimeController.h"

void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\nStopping time manipulation..." << std::endl;
        TimeController::stop();
    }
}

int main() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    try {
        TimeController::start();

        // Wait for stop signal
        while (TimeController::isRunning()) {
            std::this_thread::sleep_for(config::TIME_UPDATE_INTERVAL);
        }

        std::string dummy;
        std::getline(std::cin, dummy);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        TimeController::stop();
        return 1;
    }

    return 0;
}