#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>
#include <string>
#include <stdexcept>

// Configuration
namespace config {
    constexpr double SPEED_MULTIPLIER = 15.0;
    constexpr auto TIME_UPDATE_INTERVAL = std::chrono::milliseconds(100);
}

class TimeController {
public:
    static void start();
    static void stop();
    static bool isRunning() { return s_isRunning.load(); }

private:
    static std::atomic<bool> s_isRunning;

    static ULARGE_INTEGER getCurrentSystemTime() noexcept;
    static void setSystemTime(const ULARGE_INTEGER& time) noexcept;
    static void synchronizeTime() noexcept;
    static void accelerateTime();

    // Prevent instantiation
    TimeController() = delete;
    ~TimeController() = delete;
    TimeController(const TimeController&) = delete;
    TimeController& operator=(const TimeController&) = delete;
};

#endif