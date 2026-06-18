# Digital Content Publishing Paywall Router (ReadPass)

**B.Tech CSE 2025-29 Semester II**  
**Data Structure & Algorithms with CPP**  
**Problem Statement 27**

## 1. Project Title
ReadPass: Digital Content Publishing Paywall Router

## 2. Problem Statement
ReadPass is the subscription system behind a major news website (e.g., The New York Times, Medium). It manages millions of readers, tracks reading habits, verifies subscriptions, and dictates paywall visibility. The existing infrastructure suffers from slow subscriber retrieval, out-of-order click data processing, and an inability to efficiently map article relationships or validate discount coupons.

The new system requires fast subscriber retrieval, instant access rollback, sequential data processing, efficient coupon validation, article sorting by metrics, content mapping, shortest correlation pathfinding, and smart pricing logic to maximize conversions.

## 3. Objectives
- Design a scalable backend using appropriate C++ data structures.
- Ensure O(1) or near O(1) time complexity for core subscriber lookups.
- Maintain data integrity through LIFO access rollback and FIFO click processing.
- Build a graph-based recommendation engine for content discovery.

## 4. Architecture
The system is built as a monolithic console application (`readpass_system.cpp`) encapsulating a `ReadPassSystem` manager class. This class acts as the router, orchestrating 8 distinct functional modules, each powered by a specific data structure or algorithm. It features an interactive, menu-driven CLI for examiner evaluation.

## 5. DSA Used & Justifications
1. **Hash Map (`std::unordered_map`)**: Used for the **Subscriber Directory**. Provides O(1) average time complexity for retrieval/updates, critical for managing millions of concurrent readers.
2. **Stack (`std::stack`)**: Used for **Access Undo**. Its LIFO nature perfectly models the requirement to roll back the most recent administrative access changes (e.g., revoking a trial).
3. **Queue (`std::queue`)**: Used for the **Click Tracker**. Its FIFO nature ensures sequential processing of clickstream data.
4. **Binary Search**: Used for **Coupon Check**. Provides O(log n) efficiency to search against a sorted array of active discount codes.
5. **Merge Sort**: Used for the **Article Sorter**. Provides stable, guaranteed O(n log n) sorting by various metrics (views, engagement, monetization).
6. **Graph (Adjacency List)**: Used for the **Content Web**. Memory-efficient mapping of complex, weighted relationships between articles.
7. **Dijkstra's Algorithm**: Used for **Reading Path**. Finds the "shortest path" (strongest correlation) to recommend the optimal next article.
8. **Dynamic Programming (0/1 Knapsack)**: Used for the **Price Modeler**. Calculates the optimal combination of discount offers to maximize total conversion probability within a fixed budget.

## 6. Complexity Analysis
- **Subscriber Lookup:** Time: O(1) avg, Space: O(N) where N = users.
- **Undo Access:** Time: O(1), Space: O(A) where A = actions.
- **Coupon Check:** Time: O(log C), Space: O(1) where C = coupons.
- **Sort Articles:** Time: O(A log A), Space: O(A) where A = articles.
- **Reading Path:** Time: O((V + E) log V), Space: O(V) where V = articles, E = relationships.

## 7. Execution Steps
1. Navigate to the project directory in your terminal.
2. Compile the C++ file: `g++ -std=c++17 readpass_system.cpp -o readpass_system`
3. Run the executable: `./readpass_system`
4. Use the interactive console menu (Options 1-13) to test all functionalities.

## 8. Sample I/O
Please refer to the `sample_inputs/input.txt` and `sample_outputs/output.txt` files for complete execution traces. 
Example Output:
```text
===== READPASS MENU =====
...
Enter your choice: 4
All Subscribers:
U002 Bob Free
U001 Alice Premium
```

## 9. Results
The implementation successfully meets all 8 functional requirements. By replacing naive linear searches and arrays with Hash Maps, Graphs, and efficient sorting/pathfinding algorithms, the hypothetical performance of the paywall router is optimized to handle high-traffic, real-world publishing environments equivalent to The New York Times.

## 10. Conclusion
This project demonstrates the practical application of Data Structures and Algorithms in solving complex, real-world backend infrastructure problems. The choice of DSA directly impacts the scalability, speed, and business logic (e.g., intelligent pricing and recommendations) of a modern digital publishing platform.
