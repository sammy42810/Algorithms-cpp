# Algorithms & Data Structures (C++)

A collection of algorithm and data structure implementations in C++, progressing from foundational number theory to BFS state-space search to self-balancing trees. Each is built with a `makefile` and tested via shell scripts.

## Implementations

### PA1 — Sieve of Eratosthenes (`sieve.cpp`)
Finds all primes up to N. Optimized to iterate only up to √N and skip even numbers after 2.
- **Complexity:** O(n log log n) time, O(n) space

### PA2 — Unique Element Finder (`unique.cpp`)
Finds all unique elements in an unsorted input using sorting as a preprocessing step, then scans linearly.
- **Complexity:** O(n log n) — demonstrates how algorithm choice affects overall complexity

### PA3 — Stairclimber (`stairclimber.cpp`)
Enumerates all distinct ways to climb n stairs, taking 1, 2, or 3 steps at a time. Returns results as a `vector<vector<int>>` and prints them with zero-padded formatting.
- **Technique:** Recursive backtracking / combinatorial enumeration

### PA4 — Water Jug Puzzle (`waterjugpuzzle.cpp`)
Solves the classic water jug problem (find a sequence of pours to reach a target state) using BFS over the state space.
- **Technique:** BFS; state represented as a tuple `(a, b)` of water levels
- Prints the shortest sequence of pour operations, or "No solution" if unreachable

### PA5 — Inversion Counter (`inversioncounter.cpp`)
Counts inversions in an array — pairs `(i, j)` where `i < j` but `arr[i] > arr[j]` — via a modified merge sort.
- **Complexity:** O(n log n) — demonstrates divide-and-conquer applied to a counting problem

### PA6 — Red-Black Tree (`rbtree.h`, `tree.h`, `node.h`)
A self-balancing BST with full insertion and invariant maintenance (coloring, left/right rotations, fixup cases). Includes a tree printer for visual debugging.
- **Complexity:** Guaranteed O(log n) insert and search
- Header-only implementation using C++ templates

## Build & Test

Each PA has its own `makefile` and shell test script:

```bash
# Example: BFS water jug
cd waterjugpuzzle/
make
./waterjugpuzzle <cap_a> <cap_b> <goal_a> <goal_b>

# Inversion counter
cd inversioncounter/
make
bash test_inversioncounter.sh

# Red-Black Tree
cd rbtree/
make
bash test_rbt.sh
```

## Tech Stack

- **Language:** C++17
- **Build:** GNU Make
- **Testing:** Shell scripts with expected-output diff comparison

## Concepts Demonstrated

- Algorithm complexity analysis (O(n), O(n log n), O(n log log n))
- Recursive backtracking and combinatorial enumeration
- BFS for shortest-path state-space search
- Divide-and-conquer (merge sort, inversion counting)
- Self-balancing BSTs (Red-Black Tree rotations and fixup)
- Template / header-only data structures in C++
