#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

namespace star::common::strings
{
inline std::string startTime()
{
    // Capture the instant once so the second and millisecond fields agree.
    const auto now = std::chrono::system_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t seconds = std::chrono::system_clock::to_time_t(now);

    // Convert to local time structure (thread-safe variants; localtime is not).
    std::tm localTime{};
#ifdef _WIN32
    if (localtime_s(&localTime, &seconds) != 0)
    {
        return ""; // Or handle error as needed
    }
#else
    if (localtime_r(&seconds, &localTime) == nullptr)
    {
        return ""; // Or handle error as needed
    }
#endif

    // Buffer to hold the formatted string
    char buffer[80];

    // Format specifiers:
    // %Y - Year (e.g., 2024)
    // %m - Month (01-12)
    // %d - Day (01-31)
    // %H - Hour in 24h format (00-23)
    // %M - Minute (00-59)
    // %S - Second (00-59)
    // Underscores and hyphens are used as separators for readability and compatibility.
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", &localTime);

    std::ostringstream name;
    name << buffer << '-' << std::setw(3) << std::setfill('0') << millis.count();

    return name.str();
}

inline std::string GetPID()
{
#ifdef _WIN32
    return std::to_string(_getpid());
#else
    return std::to_string(getpid());
#endif
}

inline std::string GetStartTime()
{
    static const std::string timeValue = startTime();
    return timeValue;
}
} // namespace star::common::strings