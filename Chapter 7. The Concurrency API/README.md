# Item 35: Prefer task-based programming to thread-based.
## Things to Remember
* The `std::thread` API offers no direct way to get return values from asyn‐
chronously run functions, and if those functions throw, the program is termi‐
nated.
* Thread-based programming calls for manual management of thread exhaustion(枯竭), oversubscription, load balancing, and adaptation to new platforms.
* Task-based programming via `std::async` with the default launch policy handles most of these issues for you.

# Item 36:Specify std::launch::async if asynchronicity is essential
## Things to Remember
* The default launch policy for `std::async` permits both asynchronous and
synchronous task execution.
* This flexibility leads to uncertainty when accessing `thread_locals`, implies
that the task may never execute, and affects program logic for timeout-based
`wait` calls.
* Specify `std::launch::async` if asynchronous task execution is essential.