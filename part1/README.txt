How to run the program:

Note that two versions of the accumulator program exist: one using unsynchronized threads and the other using synchronized threads.

To compile the program that runs with synchronized threads, enter the following command:
make sync

To compile the program that runs without synchronized threads, enter the following command:
make unsync

An executable called acc should be created in the current directory. To run acc, enter the following command:

./acc <number of threads>

Usage:

acc <number of threads>

The number of threads must be a non-negative integer.
