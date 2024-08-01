# Item 10: Prefer scoped enums to unscoped enums.
## unscoped enums 缺点
- 命名空间污染： enumerator names 会泄露到包含定义它 scoped
```CPP
enum Color {red, white, blue};
auto white = false; // error: ‘auto white’ redeclared as different kind of entity
```

- unscoped enums 会隐式准换为整型，进而转换为 float
```CPP
int i = red; // ok
float f = white; // ok
```

## scoped enums
- 减少命名空间污染：不会泄露 enum names 到定义它的 scope
```CPP
enum class Color {red, white, blue};
auto white = false; // ok
```
- 拥有强类型：不会隐式转换成其他类型
```CPP
int c = Color::red; // error: cannot convert ‘main()::Color’ to ‘int’ in initialization
```

- 可以进行前置声明 forward-declarition：减少编译依赖
```CPP
enum Color; // error: use of enum ‘Color’ without previous declaration
enum class Status;
```
>> Why C++ 11 can? Because the underlying type for a scoped enum is always known. \
>> 故指定 unscoped enums 的 underlying type 便可进行前置声明
```CPP
enum Color:uint8_t; //OK
enum class Status:uint16_t; // enum class 也可以指定
```