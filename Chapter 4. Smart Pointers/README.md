# Item 18: Use unique_ptr for exclusive-ownership resource management
## Things to Remember
* `std::unique_ptr` is a small, fast, move-only smart pointer for managing
resources with exclusive-ownership semantics.
* By default, resource destruction takes place via `delete`, but custom deleters
can be specified. Stateful deleters and function pointers as deleters increase the
size of `std::unique_ptr` objects.
* By default, resource destruction takes place via delete, but custom deleters
can be specified. Stateful(有状态的) deleters and function pointers as deleters increase the
size of `std::unique_ptr` objects.
* Converting a `std::unique_ptr` to a `std::shared_ptr` is easy.

# Item 19: Use shared_ptr for shared-ownership resource management
## Things to Remember
* `std::shared_ptrs` offer convenience approaching that of garbage collection
for the shared lifetime management of arbitrary resources
* Compared to `std::unique_ptr`, `std::shared_ptr` objects are typically
twice as big, incur overhead for control blocks, and require atomic reference
count manipulations
* Default resource destruction is via `delete`, but custom deleters are supported.
The type of the deleter has no effect on the type of the `std::shared_ptr`
* Avoid creating `std::shared_ptrs` from variables of raw pointer type

# Item 20： Use weak_ptr for shared_ptr-like pointers that can dangle
## Things to Remember
* Use `std::weak_ptr` for `std::shared_ptr`-like pointers that can dangle
* Potential use cases for `std::weak_ptr` include caching, observer lists, and the
prevention of `std::shared_ptr` cycles

# Item 21: Prefer make_unique and make_shared to direct use of new
## Things to Remember
* Compared to direct use of `new`, `make` functions eliminate source code duplica‐
tion, improve exception safety, and, for `std::make_shared` and `std::allocate_shared`,
generate code that’s smaller and faster.
* Situations where use of `make` functions is inappropriate include the need to
specify custom deleters and a desire to pass braced initializers.
* For `std::shared_ptrs`, additional situations where make functions may be
ill-advised include (1) classes with custom memory management and (2) sys‐
tems with memory concerns, very large objects, and `std::weak_pt`rs that
outlive the corresponding `std::shared_ptr`s.

# Item 22: When using the Pimpl Idiom, define special member functions in the implementation file
## Things to Remember
* The Pimpl Idiom decreases build times by reducing compilation dependencies
between class clients and class implementations.
* For `std::unique_ptr` pImpl pointers, declare special member functions in
the class header, but implement them in the implementation file. Do this even
if the default function implementations are acceptable.
* The above advice applies to `std::unique_ptr`, but not to `std::shared_ptr`.

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