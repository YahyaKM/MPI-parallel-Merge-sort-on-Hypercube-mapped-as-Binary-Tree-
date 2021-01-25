# MPI-parallel-Merge-sort-on-Hypercube-mapped-as-Binary-Tree-

coursework for Parallel processing,  This program will Generate an array of integers,
and use merge sort to sort the arrays in ascending/descending order on a variable number of processors with Hypercube architecture(mapped as  Binary Tree).
to Compute the run time for each process execution and we can use that output to calculate speedup and efficiency in each case.

each process is assigned a block of n/p elements p being the number of
processes, where p < n., which it sorts internally (using merge sort) in Q((n/p) log(n/p)) time. After this, the
processes execute p phases, performing compare-split operations. At
the end of these phases, the list is sorted. During each phase, Q(n/p)
comparisons are performed to merge two blocks, and time Q(n/p) is spent communicating.

OUTPUT on 4 processors:
![4](https://user-images.githubusercontent.com/28883884/105666599-506ef180-5ee2-11eb-8153-96b30775a1c6.jpg)
