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