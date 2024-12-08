Introducing the  Smart Traffic Management System Simulator in which there are some general vehicles and some that have priority and based on their congestion level, signal timings and their path from the source to the destination level, we have used the following data structures along with the suitable alogorithms:
    
    Data Structure:
        Graph(road network)
        Priority Queue(Emergency Vehicles Routing)
        Hash Map(congestion table)
        Queue (A* path finding)
        Min Heap(Dijkstra Algorithm)

    Algorithms:
        A* Algorithm(to find the best and the shortest path for the emergency vehicle)
        Dijkstra Algorithm(to find the best path for the normal vehicles)
    

Graph for Road Network Representation:
    Each road has a start and destination instersection point and also the travel time (i.e weight from the source to destiation). There are the three tyes:
        Road is Clear (weight is as defined in the road_network.csv)
        Road is in repair(weight is 500 for this road)
        Road is Blocked(weight is 1000 for the blocked road)


    Following is the display for the Road Network:
        ![Road Netwrok Display](images/roadNetworkDisplay.png)

Priority Queue:
    When the user selects the choice to execute the emergency vehicles, a prioity queue is implemented based on the priority and reads the data from the emergency_vehicles.csv and creates a priority queue. In this files there are three types of vehicles based on there priority: 
        High( priority level is 30) 
        Medium ( priority level is 15)
        Low ( priority level is 0)

    A* algorithm is used to move these vehicles. If the path exits from the source to the destination, it shows path using the displayAStarPath. There will be no path i.e  (this indicates that all possible paths from the start node have been explored, and no valid path exists to the end node under the current constraints).

    Path Found:
        ![Path found from A to D](images/pathFound.png)

    Path Not Found:
        ![Path not found from B to F](images/pathNotFound.png)

Working of the A* Algorithm:

    1. Initial Setup
        The A* algorithm begins by creating the necessary data structures, all of fixed size (27, corresponding to nodes A to Z):

            closedList: An array of booleans, initialized to false. It keeps track of whether a node has been visited.
            gCost: An array of integers, initialized to infinity (max value). It stores the cost of the shortest path found so far from the start node to each node.
            fCost: An array of integers, initialized to infinity (max value). It represents the estimated total cost from the start node to the goal node (fCost = gCost + heuristic).
            parent: An array of characters, initialized to '\0' (null). It stores the parent node for each node, enabling path reconstruction once the goal is reached.
            openList: A priority queue, where nodes are stored and sorted by their fCost values. It manages which nodes to explore next.


    2. Initialization
        The algorithm initializes the starting node:
        Set gCost[start] = 0 (cost from the start node to itself is zero).
        Compute fCost[start] = gCost[start] + heuristic(start, goal).
        Add the starting node to the openList with its fCost.
    3. Main Loop
    The algorithm proceeds iteratively, exploring nodes until:

        The openList is empty (no more nodes to explore), or the goal node is reached.
    4. Steps in Each Iteration
        Select the Node with the Lowest fCost:

            Retrieve the node current with the lowest fCost from the openList and remove current from the openList.
            
            Check if Goal is Reached:

                If current == goal, stop. The shortest path is found.Otherwise, mark current as visited by setting  closedList[current] = true.
            Explore Neighbors:

                For each neighbor of current:
                
                Ignore Already Visited Nodes: Skip neighbors if they are in the closedList.
                Check Edge Validity: Ignore edges that do not meet constraints (e.g., weights not equal to 500 or 1000).

            Calculate the tentative gCost for the neighbor:

                tentativeGCost = gCost[current] + edgeWeight;
            
            Compare the tentative gCost to the current gCost for the neighbor:
                If tentativeGCost is smaller, update:
                    gCost[neighbor] = tentativeGCost
                    fCost[neighbor] = gCost[neighbor] + heuristic(neighbor, goal)
                    parent[neighbor] = current (record the path).
            
            Add or update the neighbor in the openList with its fCost.
    
    5. Termination
        Path Found:

            If the goal node is reached, the algorithm reconstructs the shortest path using the parent array by backtracking from the goal to the start node.

            It also displays the total cost (gCost[goal]).
        
        No Path Found:

            If the openList becomes empty and the goal is not reached, it means no valid path exists between the start and goal nodes under the given constraints.

    6. Path Reconstruction
        Once the goal node is reached:

        Start from the goal node and backtrack using the parent array.
        Add each node to the path array until the start node is reached.
        Reverse the path array to display the correct order (start → goal).

    Key Points in A Algorithm*

        Heuristic Function:
            Guides the algorithm by estimating the cost to reach the goal from the current node.Must be admissible (does not overestimate the cost) for A* to guarantee the shortest path.
            ![Heuristic Function](images/heuristicFuntion.png.png)
        
        Queue (openList):
            Ensures that nodes with the lowest fCost are explored first, optimizing the search.
        
        Early Goal Check:
            Stops as soon as the goal is reached, making A* more efficient than Dijkstra’s algorithm for goal-specific pathfinding.

        Advantages of A*:
            Faster than Dijkstra’s for goal-oriented searches.Flexible due to customizable heuristics.
        
        Disadvantages of A*:
            Memory-intensive due to the openList and fCost calculations. Can fail for graphs with negative weights or cycles.

In the class Emergency Queue, the stimulation of emergency vehciles is done using the priority queue, A* star Algorithm, heuritic function, display of these emergency vehicles path. There are two methods to make the queue, one for the priority(insertInEmergencyQueue) and the other is for the simple queue(insertAtEndInEmergencyQueue). Some other functions like readFromEmergencyCsv, and other basic functions like isEmpty, removeFromQueue are also present in this class.

Graph Class Explanation:  
    Dijkstra's Algorithm Implementation:
        This implementation of Dijkstra's Algorithm leverages a Min Heap to efficiently calculate the shortest path in a weighted graph. Below is an overview of its functionality:

        Key Components:
            Min Heap:
              The Min Heap is used to efficiently extract the vertex with the smallest distance during the algorithm's execution. It ensures optimal performance for Dijkstra's Algorithm. The key operations implemented for the Min Heap include:

                Heapify: Maintains the heap property by reorganizing elements to ensure the smallest distance remains at the root.

                Insert: Adds a new node into the heap and adjusts its position to preserve the heap property.
                
                Extract Min: Removes and returns the root node, which has the smallest distance.
                Min Heap Structure:

                Each node in the heap is represented as a HeapQueueNode, containing:
                distance: The calculated distance to the vertex.
                vertex: The vertex identifier.
        
    Graph:
        The graph is represented using adjacency lists, where each vertex maintains a list of its adjacent vertices and their respective weights. The algorithm navigates this structure to calculate shortest paths.

        Dijkstra's Algorithm Functionality
        Initialization:

            A Min Heap is initialized with a capacity matching the number of vertices in the graph.
            Arrays for distances, previous (for tracking the path), and visited (to mark processed vertices) are created.
            The distance for the starting vertex is set to 0, and this vertex is pushed into the Min Heap.
        Processing Vertices:

            The algorithm repeatedly extracts the vertex with the smallest distance from the Min Heap.
            If the vertex is already visited, it skips further processing.
            For each unvisited adjacent vertex:
            The new distance is calculated as the sum of the current vertex's distance, edge weight, and additional signal timing (if applicable).
            If the newly calculated distance is smaller than the previously stored distance:
            The distances array is updated with the smaller value.
            The previous array is updated to reflect the path.
            The adjacent vertex is added to the Min Heap.
        
        Termination:

            The algorithm terminates when either the destination vertex is reached or the Min Heap becomes empty.
            Output:

            After the traversal, the algorithm constructs the shortest path using the previous array if a valid path exists.
            If no path exists, it outputs a "Path not found" message.
            Code Highlights
        Min Heap Implementation:

            Heapify ensures the smallest distance is always at the root.
            Insert adds new vertices while maintaining heap structure.
            Extract Min efficiently retrieves the next vertex to process.
        
        Dijkstra's Algorithm Integration:
            The Min Heap is integral for prioritizing vertex processing based on distance.
            Signal timings are included in the distance calculation for real-world traffic scenarios.
            
            Usage:
                This implementation is particularly useful for:

            Vehicle Routing Problems: Optimizing paths in road networks with traffic signal timings.
            Dynamic Graphs: Adjusting for real-time changes in weights or conditions.
            Large-Scale Graphs: Efficiently processing graphs with many vertices and edges due to the Min Heap's logarithmic time complexity for insertions and deletions.
        
        Example Scenario:
        
            For a graph with vertices A, B, C, ... and weighted edges, the algorithm calculates the shortest path from a starting vertex to a destination vertex while considering additional delays (e.g., traffic signals). It efficiently handles updates using the Min Heap to ensure optimal performance.