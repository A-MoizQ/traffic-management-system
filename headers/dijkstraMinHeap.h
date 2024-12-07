#ifndef DIJKSTRAMINHEAP_H
#define DIJKSTRAMINHEAP_H

class MinHeap{
    int left(int i);
    int right(int i);
    int parent(int i);
    void heapify(int i);
public:
    struct HeapQueueNode{
        int distance; //distance to the vertex
        char vertex; 
    };
    HeapQueueNode* heapArr;
    int capacity;
    int size;
    MinHeap(int cap);
    int getMaxInt() const;
    ~MinHeap();
    void insert(int dist, char vert);
    HeapQueueNode extractMin();
    bool isEmpty() const;
};

#endif