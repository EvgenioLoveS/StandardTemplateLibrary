#include <iostream>
#include <memory>

template <typename T>
class BinaryHeapList {

private:
    struct Node {
        T _value;
        std::weak_ptr<Node> _parent;
        std::shared_ptr<Node> _leftChild;
        std::shared_ptr<Node> _rightChild;

        Node(const T& value) : _value(value) { }
    };

    std::shared_ptr<Node> _root;

    void heapifyUp(std::shared_ptr<Node> node) {
        if (node->_parent.expired()) {
            _root = node;
            return;
        }

        auto parent = node->_parent.lock();
        if (node->_value < parent->value) {
            std::swap(node->_value, parent->value);
            heapifyUp(parent);
        }
    }

    void heapifyDown(std::shared_ptr<Node> node) {
        auto smallest = node;

        if (node->_leftChild && node->_leftChild->_value < smallest->_value) {
            smallest = node->_leftChild;
        }

        if (node->_rightChild && node->_rightChild->_value < smallest->_value) {
            smallest = node->_rightChild;
        }

        if (smallest != node) {
            std::swap(node->_value, smallest->_value);
            heapifyDown(smallest);
        }
    }

public:
    void insert(const T& value) {
        auto newNode = std::make_shared<Node>(value);
        if (!_root) {
            _root = newNode;
        }
        else {
            std::shared_ptr<Node> parentNode;
            std::shared_ptr<Node> currentNode = _root;

            while (currentNode) {
                parentNode = currentNode;
                currentNode = (value < currentNode->_value) ? currentNode->_leftChild : currentNode->_rightChild;
            }

            newNode->parent = parentNode;
            if (value < parentNode->_value) {
                parentNode->_leftChild = newNode;
            }
            else {
                parentNode->_rightChild = newNode;
            }
        }

        heapifyUp(newNode);
    }

    void removeMin() {
        if (!_root) {
            return;
        }

        if (!_root->_leftChild && !_root->_rightChild) {
            _root = nullptr;
            return;
        }

        std::shared_ptr<Node> lastNode = _root;
        while (lastNode->_leftChild || lastNode->_rightChild) {
            if (lastNode->_leftChild) {
                lastNode = lastNode->_leftChild;
            }
            else {
                lastNode = lastNode->_rightChild;
            }
        }

        if (lastNode->_parent.lock()->leftChild == lastNode) {
            lastNode->_parent.lock()->leftChild = nullptr;
        }
        else {
            lastNode->_parent.lock()->rightChild = nullptr;
        }

        _root->_value = lastNode->_value;
        heapifyDown(_root);
    }

    const T& getMin() const {
        if (!_root) {
            throw std::out_of_range("Heap is empty");
        }

        return _root->_value;
    }

    bool isEmpty() const {
        return !_root;
    }
};

