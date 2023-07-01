#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

template<typename T>
class BinaryHeapArray {

private:
    std::vector<T> _heap;

    size_t parent(size_t index) { return (index - 1) / 2; }

    size_t leftchild(size_t index) { return 2 * index + 1; }

    size_t rightchild(size_t index) { return 2 * index + 2; }

    void heapifyUp(int index) {
        while (index > 0) {
            size_t parent = parent(index);
            if (_heap[parent] > _heap[index])
                break;

            std::swap(_heap[parent], _heap[index]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int size = _heap.size();
        while (true) {
            size_t largest = index;
            size_t leftChild = leftchild(index);
            size_t rightChild = rightchild(index);

            if (leftChild < size && _heap[leftChild] > _heap[largest])
                largest = leftChild;

            if (rightChild < size && _heap[rightChild] > _heap[largest])
                largest = rightChild;

            if (largest == index)
                break;

            std::swap(_heap[index], _heap[largest]);
            index = largest;
        }
    }

public:
    void insert(const T& value) {
        _heap.push_back(value);
        heapifyUp(_heap.size() - 1);
    }

    T extractMax() {
        if (_heap.empty())
            throw std::out_of_range("Heap is empty!");

        T maxElement = _heap[0];
        _heap[0] = _heap.back();
        _heap.pop_back();
        heapifyDown(0);

        return maxElement;
    }

    bool isEmpty() const {
        return _heap.empty();
    }
};

