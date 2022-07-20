# Item 23: Understand std::move and std::forward.
## Things to Remember
* `std::move performs` an unconditional cast to an rvalue. In and of itself, it
doesn’t move anything.
* `std::forward` casts its argument to an rvalue only if that argument is bound
to an rvalue.
* Neither `std::move` nor `std::forward` do anything at runtime.

# Item 24: Distinguish universal references from rvalue references.
## Things to Remember
* If a function template parameter has type `T&&` for a deduced type `T`, or if an
object is declared using `auto&&`, the parameter or object is a universal refer‐
ence.
* If the form of the type declaration isn’t precisely `type&&`, or if type deduction
does not occur, `type&&` denotes an rvalue reference.
* Universal references correspond to rvalue references if they’re initialized with
rvalues. They correspond to lvalue references if they’re initialized with lval‐
ues.

# Item 25: Use std::move on rvalue references, std::forward on universal references
## Things to Remember
* Apply `std::move` to rvalue references and `std::forward` to universal refer‐
ences the last time each is used.
* Do the same thing for rvalue references and universal references being
returned from functions that return by value.
* Never apply `std::move` or `std::forward` to local objects if they would other‐
wise be eligible for the return value optimization.

# Item 26: Avoid overloading on universal references.
## Things to Remember
* Overloading on universal references almost always leads to the universal refer‐
ence overload being called more frequently than expected.
* Perfect-forwarding constructors are especially problematic, because they’re
typically better matches than copy constructors for non-const lvalues, and
they can hijack derived class calls to base class copy and move constructors.

# Item 27: Familiarize yourself with alternatives to overloading on universal references
## Things to Remember
* Alternatives to the combination of universal references and overloading
include the use of distinct function names, passing parameters by lvalue reference-to-const, passing parameters by value, and using tag dispatch.
* Constraining templates via std::enable_if permits the use of universal ref‐
erences and overloading together, but it controls the conditions under which
compilers may use the universal reference overloads.
* Universal reference parameters often have efficiency advantages, but they typically have usability disadvantages.