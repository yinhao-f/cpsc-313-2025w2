# Optimizing Pipeline Execution: Heapsort in y86

In this lab, you are going to practice optimizing program performance. Your understanding of the y86 pipeline and how certain instructions introduce stalls and/or cancel/squash instructions will help you do this. Your job is to transform a working implementation of heapsort into a correct, but much more efficient implementation.

This is a longer lab, so we will give you two weeks to complete it (plus the reading week).

For full credit, your solution must use 66% fewer cycles when running on the pipelining with stalling implementation and 55% fewer cycles when running on the pipelining with data forwarding and branch prediction implementation.

We provide you with:

- A C language implementation of the heapsort function. You may use this as a reference.
- An implementation of a conceptually identical heapsort function implemented in y86.

Both implementations are well-commented and come with a **sample** main program and **a single short test data set**. As in prior labs, we will run additional tests for grading. **We encourage you to write your own tests**

**The autograder imposes a significant load on the PrairieLearn servers.** Therefore, we have introduced a short cooldown period (15 minutes). However, there is no reason to run the autograder to determine how well you are doing -- you can run your program on the simulator. Please take full advantage of that before submitting to the autograder (and under no circumstances should you submit the same thing to the autograder more than once). If load gets bad, we may have to increase the cooldown, **without notice**.

We provide both the C code below and as [heapsort.c](./starter_heapsort.c), a downloadable file.

In this problem, you are free to reimplement the y86 code in whatever way you choose (so long as it implements the same heapsort function: you are not allowed to implement insertion sort or mergesort instead). However, your implementation must perform better than the starting one!

Your code must do the following:

- Provide your heapsort function at the label `heapsort`.
- Respect the y86 calling conventions *from the point of view of the code that calls your heapsort function*. That is, the values of all callee save registers after the call should be the same as their values before the call.
- There is only one parameter to heapsort: the number of elements of the array to sort. The starting address of the array is at the label `heap`
- Your `heapsort` function should return, but should not halt.

Further, to receive full credit, your code must:

- Reduce the number of clock cycles taken on the 'Pipelined with stalling' implementation by at least **66%**.
- Reduce the number of clock cycles taken on the 'Pipelined with data forwarding and branch prediction' implementation by at least **55%**.

Partial marks will be given if you achieve a measurable speedup that is less than those mentioned above.

Be creative! Explore several alternative options. You might find it helps to use additional registers, change how existing registers are used, change the order of some code blocks and loops, or try other things.

You must submit your assignment here for grading, but you will find it **much** easier to debug using the course y86 simulator Unless you like relaxing by staring at the simulator, **we recommend changing both the Clock speed and the number of cycles per screen update to at least 100**.

The autograder will use its own implementation of main and test arrays, so the only thing you are required to submit is your `heapsort` function (and any required helper functions). However, you are welcome to include a main program and sample data if you wish.

Tips

- Code that sorts incorrectly won't get credit, even if it's fast! So, test your code!
- Test helper functions before testing the functions that call them.
- Make small changes and use the provided heapsort code as a control.
- Test after each change.
- Keep a record of how different changes impact your performance.
- It is easy to test out small changes by editing the code directly in the simulator. This is fine, but don't forget to reflect those changes in the file you will ultimately submit. The `load file` and `save file` buttons are handy.
- You and your partner might work on optimizing different helper functions, but as with all partner labs, you are both responsible for understanding all your code.
- Comment your code, especially your register usage.
