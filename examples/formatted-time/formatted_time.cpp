#include <iostream>
#include <sstream>
#include <ctime>
#include <string>
#include <chrono>
#include <string_view>
#include <iomanip>

namespace
{

std::string make_formatted_time(const std::string_view& fmt="%T")
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Create a tm structure from the time_t
    std::tm tm;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &now_time); // Windows specific
#else
    localtime_r(&now_time, &tm); // POSIX specific
#endif

    // Create formatted time
    std::ostringstream oss;
    oss << std::put_time(&tm, fmt.data());
    return oss.str();
}

}

int main(void)
{
    std::cout << "formatted_time: " << make_formatted_time() << std::endl; 
    std::cout << "formatted_time(\"%Y\"): " << make_formatted_time("%Y") << std::endl; 
    std::cout << "formatted_time(\"%Y-%m-%d\"): " << make_formatted_time("%Y-%m-%d") << std::endl; 
    return 0;
}
