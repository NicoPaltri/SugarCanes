
# 🌱 Sugarcanes Optimization

## 📖 The Problem

The field is divided into equal blocks. Each block can be in **exactly one** of the following states:

| State | Symbol | Description |
|-------|:------:|-------------|
| **Water** | 💧 | The block contains a water source. |
| **Humid** | 🌱 | The block is adjacent (up/down/left/right) to a water block. |
| **Dry** | 🟫 | The block is not adjacent to any water source. |

> ⚠️ Diagonal adjacency does **not** count.


## 🧩 Example

### Initial 3×3 Grid

```
🟫 🟫 🟫
🟫 🟫 🟫
🟫 🟫 🟫
```

### After Placing One Water Source (Center)

```
🟫 🌱 🟫
🌱 💧 🌱
🟫 🌱 🟫
```


## 🎯 Optimization Goal

The algorithm computes the configuration that:

1. Maximizes the number of **humid** blocks  
2. Minimizes the number of **water** blocks (if tied)


## C Strategy

Given the function:

```c
void sugarcanes(const int length, const int width);
```

### 1️⃣ Backtracking

Each cell has two possible decisions:

- Place water  
- Do not place water  

This generates a decision tree of depth:

```
rows × cols
```


### 2️⃣ Incremental Updates (O(1) per step)

Instead of recomputing the entire grid at every recursion:

- Only the selected cell and its 4 neighbors are updated  
- The humid count is updated incrementally  
- An explicit **UNDO** restores exactly the modified cells  

This keeps each recursive step in:

```
O(1)
```


### 3️⃣ Branch & Bound Pruning

To reduce the exponential search space, the algorithm computes an upper bound:

```
max_possible_humid
```

If:

```
max_possible_humid < bestHumid
```

the branch is pruned immediately.

This significantly improves practical performance.


### 📊 Complexity

Worst-case complexity:

```
O(2^(rows × cols))
```

However, pruning and bounding reduce runtime significantly for medium-sized grids.


### 🧠 Key Design Choices

- Incremental update instead of full recomputation  
- Explicit UNDO for safe backtracking  
- Upper-bound pruning heuristic  
- Memory-efficient 1D array representation  


## Python Strategy

We want to maximize the objective function:

$$ \max Z = \text{weight}_{humid} \cdot 
\sum_{r=0}^{R-1} \sum_{c=0}^{C-1} h_{r,c}-
\sum_{r=0}^{R-1} \sum_{c=0}^{C-1} w_{r,c}
$$

**Where:**

- $h_{r,c} = 1$ if block $(r,c)$ is **humid**, $0$ otherwise  
- $w_{r,c} = 1$ if block $(r,c)$ is **water**, $0$ otherwise  
- $\text{weight}_{humid} = (rows \cdot cols) + 1$ to enforce lexicographic optimization  

 **Constraints:**

$$
h_{r,c} + w_{r,c} \le 1
$$

$$
h_{r,c} \le \sum_{(i,j) \in \mathcal{N}(r,c)} w_{i,j}
$$

$$
h_{r,c}, w_{r,c} \in \{0,1\}
$$

where $\mathcal{N}(r,c)$ denotes the set of orthogonal neighbours of block $(r,c)$.


## 📈 Data Analysis & Efficiency

### The Theoretical Upper Bound: 80%

A key mathematical insight of this problem is the maximum efficiency limit.

Each water source occupies **1 block** and can hydrate at most **4 adjacent blocks**.

In an ideal infinite grid (ignoring boundary effects), every group of 5 blocks would consist of:

- 1 water block  
- 4 humid blocks  

Therefore, the theoretical upper bound for cultivable blocks is:

$$
\frac{4}{5} = 0.8
$$


### 3D Efficiency Visualization

To visualize how the algorithm performs across different grid sizes, we plotted the results where:

- **X-axis**: Number of rows  
- **Y-axis**: Number of columns  
- **Z-axis**: Efficiency  

$$
\text{Efficiency} =
\frac{\text{humidBlocks}}{rows \times cols}
$$

![Sugarcanes Efficiency Plot](images/SugarCanePlot.png)


### The Data Narrative

**The Edge Effect (Small Grids)**  

In smaller grids, efficiency is lower because boundaries restrict water sources from reaching their full 4-block potential.

- A $1 \times 1$ grid has $0\%$ efficiency  
- A $2 \times 2$ grid reaches $50\%$  

**Asymptotic Growth (Large Grids)**  

As dimensions increase, the relative impact of edges diminishes and the surface trends toward the $0.8$ asymptote.

On a $10 \times 10$ grid, the algorithm achieves approximately:

$$
76\%
$$

approaching the theoretical limit.


## 🏗 Project Structure

```
.
├── C/
│   ├── sugarcanes.c
│   └── sugarcanes.h
├── Python/
│   └── SugarCanes.ipynb
├── images/
└── README.md
```


## Conclusion

This project explores the same combinatorial optimization problem through two complementary paradigms:

- A handcrafted **backtracking + branch & bound** implementation in C  
- A formal **Integer Linear Programming** formulation in Python  

The C solution emphasizes algorithmic engineering: incremental updates, explicit state restoration, and pruning strategies to control exponential growth.

The Python ILP model highlights mathematical abstraction: a concise formulation delegates the combinatorial search to a professional solver.

The convergence toward the theoretical $0.8$ limit confirms both the structural nature of the problem and the correctness of the implementations.

Together, these approaches demonstrate how mathematical insight and careful algorithm design can transform an exponential search problem into a tractable and well-understood optimization model.