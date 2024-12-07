#include "../headers/dijkstraMinHeap.h"

MinHeap::MinHeap(int cap):capacity(cap),size(0){
    heapArr = new HeapQueueNode[capacity];
}

MinHeap::~MinHeap(){
    delete[] heapArr;
}

int MinHeap::left(int i){
    return (2*i+1);
}
int MinHeap::right(int i){
    return (2*i+2);
}
int MinHeap::parent(int i){
    return ((i-1)/2);
}

void MinHeap::heapify(int i){
    int smallest = i;
    //gets left child of i
    int l = left(i);
    //gets right child of i
    int r = right(i);
    //checks if left child has a smaller distance than the existing smallest distance and replaces it
    if (l < size && heapArr[l].distance < heapArr[smallest].distance)
            smallest = l;
    //checks if the right child has a smaller distance than the existing smallest distance and replaces it
    if (r < size && heapArr[r].distance < heapArr[smallest].distance)
        smallest = r;
    //if smallest has been replaced from i, we replace it in our min heap recursively.
    if (smallest != i) {
        HeapQueueNode temp = heapArr[i];
        heapArr[i] = heapArr[smallest];
        heapArr[smallest] = temp;
        heapify(smallest);
    }
}

void MinHeap::insert(int dist, char vert){
    if(size >= capacity){
        return;
    }
    //get the last index
    int i = size++;
    //store anonymous object into heap
    heapArr[i] = {dist,vert};
    //fixing the min heap property from the last index upto the first
    while (i != 0 && heapArr[parent(i)].distance > heapArr[i].distance) {
        HeapQueueNode temp = heapArr[i];
        heapArr[i] = heapArr[parent(i)];
        heapArr[parent(i)] = temp;
        i = parent(i);
    }

}

int MinHeap::getMaxInt() const{
    return (1 << (sizeof(int) * 8 - 1)) - 1;
}

MinHeap::HeapQueueNode MinHeap::extractMin(){
    if(size == 0){
        //if queue is empty return object with infinite distance and no vertex
        return {getMaxInt(),'\0'};
    }
    //if heap only has 1 element, return that element
    if(size == 1){
        return heapArr[--size];
    }
    HeapQueueNode root = heapArr[0];
    //replace root with last element
    heapArr[0] = heapArr[--size];
    //recursively heapify the heap
    heapify(0);
    return root;
}


bool MinHeap::isEmpty() const{
    return (size == 0);
}

