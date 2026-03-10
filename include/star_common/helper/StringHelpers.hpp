#pragma once

#include <string>

namespace star::common::strings
{
inline std::string startTime()
{
    std::time_t now = std::time(nullptr);
    // Convert to local time structure
    std::tm *localTime = std::localtime(&now);

    // Check for potential errors with localtime
    if (localTime == nullptr)
    {
        return ""; // Or handle error as needed
    }

    // Buffer to hold the formatted string
    char buffer[80];

    // Format the time into the buffer
    // Format specifiers:
    // %Y - Year (e.g., 2024)
    // %m - Month (01-12)
    // %d - Day (01-31)
    // %H - Hour in 24h format (00-23)
    // %M - Minute (00-59)
    // %S - Second (00-59)
    // Underscores and hyphens are used as separators for readability and compatibility.
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", localTime);

    return std::string(buffer);
}

inline std::string GetStartTime()
{
    static const std::string timeValue = startTime();
    return timeValue;
}
} // namespace star::common