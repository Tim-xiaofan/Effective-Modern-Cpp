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