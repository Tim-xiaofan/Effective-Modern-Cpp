| Characteristic                 | CountDownLatch             | Barrier                      |
| ------------------------------ | -------------------------- | ----------------------------- |
| **Purpose**                    | Wait until tasks complete  | Synchronize at a common point |
| **One-Time vs. Reusable**      | One-time use                | Reusable                      |
| **Usage Scenarios**            | Ensure tasks completion    | Stage synchronization         |
| **Behavior**                   | `count_down` releases      | `wait` releases all threads   |
|                                | when count reaches zero    | simultaneously                |
| **Reset Capability**           | Cannot be reset             | Can be reset for reuse        |

