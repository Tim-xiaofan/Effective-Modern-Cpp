#include <iostream>
#include <sstream>
#include <ctime>
#include <string>
#include <chrono>
#include <string_view>
#include <iomanip>
#include <stdexcept>

namespace
{

using namespace std::chrono;

std::string make_formatted_time(const std::tm &tm, 
            const std::string_view& fmt )
{

    // Create formatted time
    std::ostringstream oss;
    oss << std::put_time(&tm, fmt.data());
    return oss.str();
}

std::string make_formatted_time(system_clock::time_point now = system_clock::now(),
            const std::string_view& fmt="%T")
{
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Create a tm structure from the time_t
    std::tm tm{};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &now_time); // Windows specific
#else
    localtime_r(&now_time, &tm); // POSIX specific
#endif


    return make_formatted_time(tm, fmt);
}

std::tm parse_formatted_time(const std::string_view& formatted_time,
            const std::string_view& fmt) 
{
    std::tm tm{};
    std::istringstream ss(formatted_time.data());
    ss >> std::get_time(&tm, fmt.data());

    if (ss.fail()) {
        throw std::invalid_argument("get_time failed");
    }

    return tm;
}

}

int main(void)
{
    {
        std::cout << "formatted_time: " << make_formatted_time() << std::endl; 
        std::cout << "formatted_time(\"%Y\"): " << 
            make_formatted_time(system_clock::now(), "%Y") 
            << std::endl; 
        std::cout << "formatted_time(\"%Y-%m-%d\"): " << 
            make_formatted_time(system_clock::now(), "%Y-%m-%d") << std::endl; 
    }

    {
        std::string_view fmt_time{"2011-02-28 11:54:25"};
        std::string_view fmt{"%Y-%m-%d %H:%M:%S"};
        auto tm = parse_formatted_time(fmt_time, fmt);
        std::cout << "parsed: " 
            << make_formatted_time(tm, fmt)
            << std::endl;;
    }
    return 0;
}

