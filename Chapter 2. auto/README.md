# Item 5: Prefer auto to explicit type declarations
## Things to Remember
* auto variables must be initialized, are generally immune to type mismatches
that can lead to portability or efficiency problems, can ease the process of
refactoring, and typically require less typing than variables with explicitly
specified types.
* auto-typed variables are subject to the pitfalls described in Items 2 and 6.

# Item 6: Use the explicitly typed initializer idiom when auto deduces undesired types
## Things to Remember
* "Invisible" proxy types can cause auto to deduce the "wrong" type for an ini‐
tializing expression.
* The explicitly typed initializer idiom forces auto to deduce the type you want
it to have.