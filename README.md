Problem statement:

1 Introduction
This homework will consist of two discrete parts. In the first part, you will determine the parameters of the
second-level memory cache through time measurements. In the second part, you will implement a simple
user-level thread (ULT) library for Linux.
2 Part 1 - Determining L2 Cache Parameters
In class, it was discussed how the cache works to mitigate the time costly of main memory data access. The
cache can be characterized by three parameters:
• Size of the Cache Block/Line
• Cache Size
• Associativity
As the cost to access data from the Main Memory is several orders of magnitude higher than from the cache,
a good program design is one that most of the accessed data is available in the cache, avoiding several expensive
accesses to the Main Memory. Cache’s design exploits the principle of locality, which states that
data accessed now has a higher chance to be accessed again in the near future (temporal locality), and that
data blocks near the one accessed now have, also, higher chances to be accessed in the near future (spacial
locality).
Different processors, even from the same manufacturer, have different Cache designs and Memory Hierarchies,
in terms of Cache levels (L1, L2, L3, shared L3, etc). Refer to the Processor’s Manual to know the
Cache Parameters and Memory Hierarchy of your CPU.
For this part of the homework, you are asked to design a program that will allow you to extract the L2
Cache parameters. You should be able to determine:
1. Cache Block/Line Size
2. Cache Size
3. Cache Miss Penalty (the time spent when data is not cached and must be fetched from main memory)
1CS 519 - Homework 0 Due by February 12th, 2015
The following list outlines some ideas that will help you to achieve this part:
• Since the cache is transparent to the application programmer, there is no direct method to measure the
cache size. Therefore, the cache should be measured indirectly.
• The way we achieve this is by repeatedly running a program that accesses a large array with different
patterns to lead to predictable capacity misses on the cache. By measuring the running time of the
program we can infer the cache size.
• Assume that the cache line and size are a power of two.
• Make sure that the memory used in the program is already mapped into the physical memory before
measuring the running time. This can be achieved by traversing the array once before starting the
measurement.
• The array you traverse in your program is allocated at virtual addresses, while the L2 cache whose
parameters you are measuring maps physical addresses. Your approach can ignore this since it will
not significantly affect the result.
2.1 Requirements
We will provide a Makefile and simple template. You should do all of your implemtation inside the include
files analyzecache.c and analyzecache.h. The Makefile will produce a binary called analyzecache. The
output of this program should be:
Cache Block/Line Size: XXX B
Cache Size: XXX KB
Cache Miss Penalty: XXX us
Deviations from this output format will not be accepted.
You should also include a short report about your methodology and results (namely, the size of the cache line
and the size of the cache, and how you determined them). This report should also contain a short description
about each C file and the main function’s input argument(s). Sometimes it is easier to analyze the data
collected by your memory accesses in a chart, submit it if you have created one.

Solution: 
Refer to Report.pdf