# Item 7: Distinguish between () and {} when creating objects
## Things to Remember
* Braced initialization is the most widely usable initialization syntax, it prevents
narrowing conversions, and it’s immune to C++’s most vexing parse.
* During constructor overload resolution, braced initializers are matched to
`std::initializer_list` parameters if at all possible, even if other construc‐
tors offer seemingly better matches.
* An example of where the choice between parentheses and braces can make a
significant difference is creating a `std::vector<numeric type>` with two
arguments
* Choosing between parentheses and braces for object creation inside templates
can be challenging

# Item 8: Prefer nullptr to 0 and NULL
## Things to Remember
* Prefer nullptr to 0 and NULL
* Avoid overloading on integral and pointer types

# Item 9: Prefer alias declarations to typedefs
## Things to Remember
* typedefs don’t support templatization, but alias declarations do.
* Alias templates avoid the `“::type”` suffix and, in templates, the `“typename”` prefix often required to refer to typedefs.
* C++14 offers alias templates for all the C++11 type traits transformations.

# Item 10: Prefer scoped enums to unscoped enums
## Things to Remember
* C++98-style `enums` are now known as `unscoped enums`.
* Enumerators of `scoped enums` are visible only within the `enum`. They convert
to other types only with a cast.
* Both scoped and unscoped `enums` support specification of the underlying type.
The default underlying type for scoped enums is int. Unscoped enums have no
default underlying type.
* Scoped `enums` may always be forward-declared. Unscoped `enums` may be
forward-declared only if their declaration specifies an underlying type.

# Item 11: Prefer deleted functions to private undefined ones
## Things to Remember
* Prefer deleted functions to private undefined ones.
* Any function may be deleted, including non-member functions and template
instantiations.

# Item 12: Declare overriding functions override
## Things to Remember
* Declare overriding functions override
* Member function reference qualifiers make it possible to treat lvalue and
rvalue objects (*this) differently

# Item 13: Prefer const_iterators to iterators
## Things to Remember
* Prefer `const_iterators` to `iterators`
* In maximally generic code, prefer non-member versions of begin, end,
rbegin, etc., over their member function counterparts