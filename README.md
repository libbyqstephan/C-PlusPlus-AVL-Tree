# C-PlusPlus-AVL-Tree
An implemented self-balancing binary search tree using the AVL method

This program is an assignment from an Advanced Data Structures class. Using object-oriented programming, this binary search tree is self-balancing which can reduce search time.

The program contains a driver in the main meant to showcase each function available for the user.
The functions available are:
- insert
- search
- remove
- count (returns number of nodes in the tree)

Additionally, the program contains a method to print the tree. The orientation of the tree will be displayed horizontally such that the root is to the left and the leaves to the right. Note that if a node has one child, that child will always appear to the right (above) the parent despite its value. However, if a node has two children, they will be ordered accordingly. This is simply a constraint of the print function. The tree itself will have correct orientations.
