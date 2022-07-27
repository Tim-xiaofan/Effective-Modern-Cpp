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

# Item 37: Make std::threads unjoinable on all paths.
## Things to Remember
* Make `std::thread`s unjoinable on all paths.
* `join`-on-destruction can lead to difficult-to-debug performance anomalies.
* `detach`-on-destruction can lead to difficult-to-debug undefined behavior.
* Declare `std::thread` objects last in lists of data members.

# Item 38: Be aware of varying thread handle destructor behavior
## Things to Remember
* Future destructors normally just destroy the future’s data members.
* The final future referring to a shared state for a non-deferred task launched
via `std::async` blocks until the task completes.

# Item 39:Consider void futures for one-shot event communication.
## Things to Remember
* For simple event communication, condvar-based designs require a superflu‐
ous mutex, impose constraints on the relative progress of detecting and react‐
ing tasks, and require reacting tasks to verify that the event has taken place.
* Designs employing a flag avoid those problems, but are based on polling, not
blocking.
* A condvar and flag can be used together, but the resulting communications
mechanism is somewhat stilted.
* Using `std::promise`s and futures dodges these issues, but the approach uses
heap memory for shared states, and it’s limited to one-shot communication.

# Item 40: Use std::atomic for concurrency, volatile for special memory.
## Things to Remember
* `std::atomic` is for data accessed from multiple threads without using
mutexes. It’s a tool for writing concurrent software
* `volatile` is for memory where reads and writes should not be optimized
away. It’s a tool for working with special memory.