# Item 31: Avoid default capture modes.
## Things to Remember
* Default by-reference capture can lead to dangling references.
* Default by-value capture is susceptible(易受影响的) to dangling pointers (especially this),
and it misleadingly suggests that lambdas are self-contained.