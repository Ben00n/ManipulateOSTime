#include "TimeController.h"

std::atomic<bool> TimeController::s_isRunning(false);

ULARGE_INTEGER TimeController::getCurrentSystemTime() noexcept {
    SYSTEMTIME currentTime;
    FILETIME fileTime;
    GetSystemTime(&currentTime);
    SystemTimeToFileTime(&currentTime, &fileTime);

    ULARGE_INTEGER time;
    time.LowPart = fileTime.dwLowDateTime;
    time.HighPart = fileTime.dwHighDateTime;
    return time;
}

void TimeController::setSystemTime(const ULARGE_INTEGER& time) noexcept {
    FILETIME fileTime;
    fileTime.dwLowDateTime = time.LowPart;
    fileTime.dwHighDateTime = time.HighPart;

    SYSTEMTIME systemTime;
    FileTimeToSystemTime(&fileTime, &systemTime);

    if (!SetSystemTime(&systemTime)) {
        std::cerr << "[-] Failed to set system time. Ensure administrator privileges." << std::endl;
    }
}

void TimeController::synchronizeTime() noexcept {
    std::cout << "[+] Starting time synchronization..." << std::endl;

    // Execute sync command silently but capture errors
    FILE* pipe = _popen("w32tm /resync >nul", "r");
    if (!pipe) {
        std::cerr << "[-] Failed to execute time sync command." << std::endl;
        return;
    }
    _pclose(pipe);

    std::cout << "[+] Time synchronization complete." << std::endl;
}

void TimeController::accelerateTime() {
    const auto programStart = std::chrono::steady_clock::now();
    const ULARGE_INTEGER startFakeTime = getCurrentSystemTime();

    std::cout << "Time manipulation started. Press Ctrl+C to stop." << std::endl;

    while (s_isRunning) {
        const auto now = std::chrono::steady_clock::now();
        const auto elapsedRealMilliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - programStart).count();

        ULARGE_INTEGER fakeTimeValue;
        fakeTimeValue.QuadPart = startFakeTime.QuadPart +
            static_cast<ULONGLONG>(elapsedRealMilliseconds * config::SPEED_MULTIPLIER * 10000);

        setSystemTime(fakeTimeValue);
        std::this_thread::sleep_for(config::TIME_UPDATE_INTERVAL);
    }
}

void TimeController::start() {
    s_isRunning = true;
    std::thread([]() {
        try {
            accelerateTime();
        }
        catch (const std::exception& e) {
            std::cerr << "Error in time acceleration thread: " << e.what() << std::endl;
            s_isRunning = false;
        }
        }).detach();
}

void TimeController::stop() {
    if (s_isRunning.exchange(false)) {
        std::this_thread::sleep_for(config::TIME_UPDATE_INTERVAL);
        synchronizeTime();
    }
}