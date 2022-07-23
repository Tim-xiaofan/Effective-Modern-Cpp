# Item 31: Avoid default capture modes.
## Things to Remember
* Default by-reference capture can lead to dangling references.
* Default by-value capture is susceptible(易受影响的) to dangling pointers (especially this),
and it misleadingly suggests that lambdas are self-contained.

# Item 32: Use init capture to move objects into closures.
## Things to Remember
* Use C++14’s init capture to move objects into closures.
* In C++11, emulate init capture via hand-written classes or `std::bind`.

# Item 33: Use decltype on auto&& parameters to std::forward them.
## Things to Remember
* Use `decltype` on `auto&&` parameters to `std::forward` them.

# Item 34: Prefer lambdas to std::bind.
## Things to Remember
* Lambdas are more readable, more expressive, and may be more efficient than
using `std::bind`.
* In C++11 only, `std::bind` may be useful for implementing move capture or
for binding objects with templatized function call operators.