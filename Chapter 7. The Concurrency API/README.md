# Item 35: Prefer task-based programming to thread-based.
## Things to Remember
* The `std::thread` API offers no direct way to get return values from asyn‐
chronously run functions, and if those functions throw, the program is termi‐
nated.
* Thread-based programming calls for manual management of thread exhaustion(枯竭), oversubscription, load balancing, and adaptation to new platforms.
* Task-based programming via `std::async` with the default launch policy handles most of these issues for you.