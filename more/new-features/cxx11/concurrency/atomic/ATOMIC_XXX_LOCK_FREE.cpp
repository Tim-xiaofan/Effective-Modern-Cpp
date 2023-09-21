#include <atomic>
// C++17 之前可以用标准库为各个原子类型定义的 ATOMIC_xxx_LOCK_FREE 宏来判断该类型是否无锁
// ，值为 0 表示原子类型是有锁的，
// 为 2 表示无锁，
// 为 1 表示运行时才能确定

static_assert(ATOMIC_BOOL_LOCK_FREE == 2);
static_assert(ATOMIC_CHAR_LOCK_FREE == 2);
static_assert(ATOMIC_SHORT_LOCK_FREE == 2);
static_assert(ATOMIC_LLONG_LOCK_FREE == 2);
static_assert(ATOMIC_POINTER_LOCK_FREE == 2);

int main()
{
	return 0;
};
