🌱 Sugarcanes Optimization Problem

📌 Overview



This project implements an optimization algorithm to determine the best placement of water sources in a rectangular sugarcane field.



The objective is:



Maximize the number of humid blocks



Among equivalent solutions, minimize the number of water sources



The problem is solved using a branch-and-bound backtracking approach with incremental state updates and pruning heuristics.



🌾 Problem Description



The field is divided into equal rectangular blocks.

Each block can be in exactly one of the following states:



Value	State	Description

0	dry	Not adjacent to any water source

1	water	Contains a water source

2	humid	Adjacent (up/down/left/right) to a water source



⚠️ Diagonal adjacency does NOT count.



🎯 Optimization Goal



Given the field dimensions:



void sugarcanes(const int length, const int width);



The algorithm finds the configuration of water sources that:



Maximizes humid blocks



Minimizes water blocks (secondary objective)



📐 Example

3×3 Grid



Initial empty field:



. . .

. . .

. . .



Placing a water source in the center:



. H .

H W H

. H .



Legend:



W = water



H = humid



. = dry



Only orthogonal neighbors become humid.



🧠 Algorithm Strategy

1️⃣ Backtracking (Full Exploration)



Each cell has two possibilities:



Place water



Do not place water



This generates a binary decision tree of depth rows × cols.



2️⃣ Incremental State Updates



Instead of recomputing the entire grid at each step:



Only the modified cell and its 4 neighbors are updated.



State restoration (UNDO) restores exactly 5 cells.



Humid count is updated incrementally.



This makes each recursive step O(1).



3️⃣ Branch \& Bound Pruning



To reduce the exponential search space:



The algorithm computes an upper bound on the maximum possible humid blocks from the current partial solution.



If that bound is worse than the best solution found so far, the branch is pruned.



This significantly improves performance in practice.



⚙️ Project Structure

.

├── sugarcanes.h

├── sugarcanes.c

└── README.md

sugarcanes.h



Defines:



Grid states (dry, water, humid)



Matrix structure



Public function sugarcanes()



sugarcanes.c



Implements:



Grid handling utilities



Incremental recomputation logic



Recursive solver with pruning



Output formatting



🚀 Compilation



Using GCC:



gcc -O2 sugarcanes.c -o sugarcanes



Then call sugarcanes(length, width) from your main().



📊 Complexity



Worst-case complexity:



O(2^(rows \* cols))



However, pruning and bounding reduce the practical runtime significantly for medium-sized grids.



🧩 Key Design Choices



Incremental update instead of global recomputation



Explicit UNDO for safe backtracking



Bounding heuristic for pruning



Memory-efficient 1D array representation



🔮 Possible Extensions



ILP / MaxSAT formulation



Parallel search



Dynamic programming optimization



Visualization interface



Theoretical pattern analysis of optimal layouts



📚 Educational Value



This project is a compact example of:



Combinatorial optimization



Branch \& Bound techniques



State-space search



Efficient backtracking



Pruning strategies



It demonstrates how careful algorithm design dramatically improves performance compared to naive brute-force exploration.

