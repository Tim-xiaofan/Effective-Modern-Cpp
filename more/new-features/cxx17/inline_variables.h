#ifndef INLINE_VARIABLES_H_
#define INLINE_VARIABLES_H_
#include <string>
#include <iostream>
#include <pthread.h>

/// C++17之前只有内联函数，C++17开始新增内联变量。
/// 在C++17之前，类中的静态变量是不能在头文件中初始化的，需要在源文件里面进行初始化。
/// 有了内联变量之后，就可以在头文件中进行初始化了
class Foo
{
	public:
		static int numbers(void) { return numbers_; }

	private:
		inline static int numbers_ = 32;
};

/// 自从C++17开始，你可以在头文件中以inline的方式 定义 全局变量/对象
/// 只要一个编译单元内没有重复的定义即可。此例中的定义即使被多个编译单元使用， 也会指向同一个对象。
inline Foo myGlobalObj;  // 即使被多个CPP文件包含也OK


/// 对于静态成员，constexpr修饰符现在隐含着inline。 自从C++17起，如下声明 定义 了静态数据成员n：
struct D
{
    static constexpr int n = 5; // C++11/C++14: 声明
                                // 自从C++17起: 定义
	// 等价于
	// inline static constexpr int n = 5; 
};

/// 通过使用thread_local你可以为每个线程创建一个内联变量
struct MyData {

	MyData()
	{
		std::cout << "MyData ctor in thread-" << pthread_self() << std::endl; 
	}

    inline static std::string gName = "global";             // 整个程序中只有一个
    inline static thread_local std::string tName = "tls";   // 每个线程有一个
    std::string lName = "local";                            // 每个实例有一个
    void print(const std::string& msg) const {
        std::cout << msg << '\n';
        std::cout << "- gName: " << gName << '\n';
        std::cout << "- tName: " << tName << '\n';
        std::cout << "- lName: " << lName << '\n';
    }
};

inline thread_local MyData myThreadData;    // 每个线程一个对象

#endif
