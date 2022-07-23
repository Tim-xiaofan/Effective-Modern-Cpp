#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <functional>
#include <unistd.h>

using std::endl;
using std::cout;
using std::cerr;

using Time = std::chrono::steady_clock::time_point;

enum class Sound {Beep, Siren, Whistle};

using Duration = std::chrono::steady_clock::duration;

template<std::intmax_t resolution>
std::ostream &operator<<(
    std::ostream &stream,
    const std::chrono::duration<
        std::intmax_t,
        std::ratio<std::intmax_t(1), resolution>
    > &duration)
{
    const std::intmax_t ticks = duration.count();
    stream << (ticks / resolution) << '.';
    std::intmax_t div = resolution;
    std::intmax_t frac = ticks;
    for (;;) {
        frac %= div;
        if (frac == 0) break;
        div /= 10;
        stream << frac / div;
    }
    return stream;
}

template<typename Clock, typename Duration>
std::ostream &operator<<(
    std::ostream &stream,
    const std::chrono::time_point<Clock, Duration> &timepoint)
{
    Duration ago = timepoint.time_since_epoch();
    return stream << ago;
}

// at time t, make sound s for duration d
void setAlarm(Time t, Sound s, Duration d)
{
	cout << "t:" <<  t << endl; 
}

#ifdef ERROR2
enum class Volume { Normal, Loud, LoudPlusPlus };
void setAlarm(Time t, Sound s, Duration d, Volume v)
{
	cout << "t:" <<  t << endl; 
}
using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);
#endif

auto setSoundL = 
	[](Sound s)
{
	using namespace std::chrono;
#if __cplusplus >= 201402
	using namespace std::literals; //for C++14 suffixes
	setAlarm(steady_clock::now() + 1h, s, 30s);
#else
	setAlarm(steady_clock::now() + hours(1),
				s,
				seconds(30));
#endif
};

using namespace std::chrono;
using namespace std::placeholders; // needed for use of "_1
#if __cplusplus >= 201402
using namespace std::literals; //for C++14 suffixes
auto setSoundB = std::bind(setAlarm, 
#ifdef ERROR1
			steady_clock::now() + 1h, //  incorrect!
#else
			std::bind(std::plus<>(), steady_clock::now(), 1h), //FIXME: does no work
#endif
			_1, 
			30s);
#else // std=C++11
auto setSoundB = std::bind(setAlarm,
#ifdef ERROR1
			steady_clock::now() + hours(1),
#else
			std::bind(std::plus<steady_clock::time_point>(),
				steady_clock::now(),
				hours(1)),
#endif
			_1, 
			seconds(30));
#endif

int main(int argc, char * argv[])
{
	cout << "__cplusplus: " << __cplusplus << endl;

	{
		cout << "std::bind vs lambda" << endl;
		sleep(2);
		time_point<std::chrono::steady_clock> now 
			= steady_clock::now();
		cout << "now + 1h: \n  " << now + hours(1) << "\n";

		setSoundL(Sound::Beep); // lambda
		setSoundB(Sound::Beep); // bound
	}
	return 0;
}
