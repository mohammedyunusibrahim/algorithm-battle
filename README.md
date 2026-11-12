# Algorithm Battle – "Which Data Structure Wins?"

**Institute:** SRM Institute of Science and Technology, Tiruchirappalli
**Course:** 21CS201J – Data Structures and Algorithms | CLA-2, Assessment Task 3
**Department:** CSE-AIML | B.Tech, Semester III | AY 2026-2027
**Student:** Mohamed yunus ibrahim KS
**Register No:** RA2511026050176
**Application:** Library Book Search (by Book ID)

---

## Part A – Problem Selection

**a. What is the problem?**
A library holds thousands of books, each with a unique Book ID. Staff and students need to store new books and quickly find a book's details (title, author) from its ID.

**b. Why is efficient searching required?**
A linear scan costs O(n) comparisons per lookup, which becomes slow as the catalogue grows and many users search at the same time. A structure with O(log n) or O(1) lookup keeps the system responsive.

**c. Why are BST, AVL and Hashing suitable for comparison?**
- **BST:** simple ordered structure, O(log n) on average, but can degrade to O(n) for sorted input.
- **AVL Tree:** self-balancing BST that guarantees O(log n) height through rotations, at the cost of extra work during insertion.
- **Hashing (linear probing):** O(1) average lookup, but collisions and clustering can slow it down, and it gives no sorted order.

They represent three different trade-offs, so they make a meaningful comparison.

---

## Part B – Implementation

File: `main.cpp` (C++17)

| Structure | Operations implemented |
|---|---|
| BST | Insertion, Search, Inorder / Preorder / Postorder traversal, Height |
| AVL Tree | Insertion with LL, RR, LR, RL rotations, Search, Inorder / Preorder / Postorder traversal, Height |
| Hashing | h(key) = key % 53, Insertion and Search with Linear Probing, collision counting |

**Compile:** `g++ -std=c++17 -o battle main.cpp`
**Run:** `./battle`

The program also has a Technical Defense Mode: enter any Book ID and it shows how BST, AVL and Hashing handle it.

---

## Part C – Mini Experiment

- **Dataset:** 40 unique Book IDs (1001–1098) with title and author, inserted in the same order into all three structures.
- **Successful search:** all 40 stored keys.
- **Unsuccessful search:** 10 keys not in the dataset.
- **Insertion:** the 40 initial inserts plus 5 new keys.
- **Counting rule:** one comparison per node visited (trees) or per slot examined (hashing).

| Experiment | BST | AVL | Hashing |
|---|---|---|---|
| Successful search – avg comparisons | 4.58 | 4.58 | 1.55 |
| Successful search – max comparisons | 6 | 6 | 6 |
| Unsuccessful search – avg comparisons | 5.40 | 5.40 | 5.90 |
| Unsuccessful search – max comparisons | 6 | 6 | 17 |
| Insertion (initial 40) – avg comparisons | 3.58 | 3.58 | 1.55 |
| Insertion (5 new keys) – avg comparisons | 5.20 | 5.20 | 2.80 |
| Tree height after initial 40 | 6 | 6 | – |
| Tree height after 5 more inserts | 7 | 6 | – |
| AVL rotations (build / total) | – | 0 / 1 | – |
| Collisions (extra probes) | – | – | 31 |
| Insertions that collided | – | – | 14 |

Hash load factor after all inserts: 0.85



![Output](output.jpg)



---

## Part D – Algorithm Battle

| Parameter | BST | AVL | Hashing |
|---|---|---|---|
| Search comparisons | 4.58 avg, 6 max | 4.58 avg, 6 max | 1.55 avg, 17 max (unsuccessful) |
| Insertion observations | 3.58 avg initially, 5.20 for new keys | Same comparisons, plus 1 rotation | 1.55 avg initially, 2.80 for new keys |
| Height | 6, then 7 | 6, then 6 | – |
| Collisions | – | – | 31 extra probes, 14 insertions collided |
| Overall observation | Easy to build, but the tree became taller (6 to 7) after adding new books | Stayed at height 6 by doing one rotation | Fastest on average, but one failed search took 17 steps |

---

## Part E – Final Verdict

**a. Winner for Library Book Search:** Hashing with Linear Probing

**b. Justification from my own observations:**
In my experiment, Hashing found a book in 1.55 comparisons on average, while BST and AVL needed 4.58. Insertion was also faster in Hashing (1.55 vs 3.58 at first). The problem is that many keys landed close together at a load factor of 0.85, so one unsuccessful search took 17 steps, while the trees never went above 6. If the library also wants books listed in sorted order, AVL is the better choice, because it stayed at height 6 while BST grew to 7.
