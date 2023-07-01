#include <cmath>
#include <stdexcept>

template<class T>
class Node {
public:
    Node() { }
    ~Node() {
        delete prev;
        delete next;
        delete[] data;
    }

    int count = 0;
    Node<T> *prev = nullptr;
    Node<T> *next = nullptr;
    T *data = new T[4];
};

template<class T>
class DequeIterator {
private:
    Node<T> *_node;
    int _index;

public:
    DequeIterator() {}

    DequeIterator(Node<T> *node) : _node(node) {}

    DequeIterator(Node<T> *node, size_t index) : _node(node), _index(index) {}

    ~DequeIterator() {}

    DequeIterator<T> begin() const {
        Node<T> *firstNode = _node;
        while (firstNode->prev) {
            firstNode = firstNode->prev;
        }
        return DequeIterator<T>(firstNode, 0);
    }

    DequeIterator<T> end() const {
        Node<T> *lastNode = _node;
        while (lastNode->next) {
            lastNode = lastNode->next;
        }
        return DequeIterator<T>(lastNode, lastNode->count);
    }

    T &operator*() const {
        return _node->data[_index];
    }

    T *operator->() const {
        return &_node->data[_index];
    }

    DequeIterator<T> begin() {
        Node<T> *firstNode = _node;
        while (firstNode->prev) {
            firstNode = firstNode->prev;
        }
        return DequeIterator<T>(firstNode, 0);
    }

    DequeIterator<T> end() {
        Node<T> *lastNode = _node;
        while (lastNode->next) {
            lastNode = lastNode->next;
        }
        return DequeIterator<T>(lastNode, lastNode->count);
    }

    DequeIterator &operator++() {
        if (_index + 1 < _node->count) {
            _index++;
        } else {
            _node = _node->next;
            _index = 0;
        }
        return *this;
    }

    DequeIterator operator++(int) {
        DequeIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    DequeIterator &operator--() {
        if (_index > 0) {
            _index--;
        } else {
            _node = _node->prev;
            _index = _node->count - 1;
        }
        return *this;
    }

    DequeIterator operator--(int) {
        DequeIterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const DequeIterator &other) const {
        return (_node == other._node) && (_index == other._index);
    }

    bool operator!=(const DequeIterator &other) const {
        return !(*this == other);
    }
};


template<class T>
class Deque : public DequeIterator<T> {
private:
    Node<T> *m_head;
    Node<T> *m_tail;
    size_t m_capacity = 0;
    size_t m_size = 0;

public:
    DequeIterator<T> begin() {
        Node<T>* firstNode = m_head;
        return DequeIterator<T>(firstNode, 0);
    }

    DequeIterator<T> end() {
        Node<T>* lastNode = m_tail;
        return DequeIterator<T>(lastNode, lastNode->count);
    }

    Deque() {}

    ~Deque() {}

    void push_back(const T& addItem);
    void push_front(const T& addItem);
    void pop_back();
    void pop_front();
    void resize(size_t new_size);

    bool isEmpty() {
        return m_size == 0;
    }

    int getSize() {
        return m_size;
    }

    void clear() {
        m_capacity = 0;
        m_size = 0;
        m_head = nullptr;
        m_tail = nullptr;
    }

    T& back() {
        return m_tail->data[m_tail->count - 1];
    }

    T& front() {
        return m_head->data[0];
    }

    T& operator[](size_t index) {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* node = m_head;
        size_t count = 0;
        while (node != nullptr && count + node->count <= index) {
            count += node->count;
            node = node->prev;
        }
        return node->data[index - count];
    }
};

template<class T>
void Deque<T>::push_back(const T& addItem) {
    if (m_capacity == 0) {
        m_head = new Node<T>;
        m_tail = m_head;
        m_tail->data[0] = addItem;
        m_tail->count++;
        m_capacity++;
    } else {
        if (m_tail->count < 4) {
            m_tail->data[m_tail->count] = addItem;
            m_tail->count++;
        } else {
            m_tail->prev = new Node<T>;
            m_tail->prev->next = m_tail;
            m_tail = m_tail->prev;
            m_tail->data[m_tail->count] = addItem;
            m_tail->count++;
            m_capacity++;
        }
    }
    m_size++;
}

template<class T>
void Deque<T>::push_front(const T& addItem) {
    if (m_capacity == 0) {
        m_tail = m_head = new Node<T>;
        m_head->data[0] = addItem;
        m_head->count++;
        m_head->prev = m_tail;
        m_capacity++;
    } else {
        if (m_head->count < 4) {
            for (auto i = 0; i < m_head->count; i++) {
                auto tmp = m_head->data[i + 1];
                m_head->data[i + 1] = m_head->data[i];
            }
            m_head->data[0] = addItem;
            m_head->count++;
        } else {
            m_head->next = new Node<T>;
            m_head->next->prev = m_head;
            m_head = m_head->next;
            m_head->data[0] = addItem;
            m_head->count++;
            m_capacity++;
        }
    }
    m_size++;
}

template<class T>
void Deque<T>::pop_back() {
    if (m_tail->count > 1) {
        m_tail->data[--m_tail->count] = T();
    } else {
        Node<T>* tmp = m_tail;
        m_tail = m_tail->prev;
        if (m_tail) {
            delete tmp;
            m_tail->next = nullptr;
        } else {
            delete[] tmp->data;
            delete tmp;
            m_tail = m_head = nullptr;
            m_capacity = m_size = 0;
        }
    }
    m_size--;
}

template<class T>
void Deque<T>::pop_front() {
    if (m_head->count <= 1) {
        Node<T>* tmp = m_head;
        m_head = m_head->prev;
        delete tmp;
        m_capacity--;
    } else {
        m_head->data[0] = rand();
        for (auto i = 1; i < m_head->count; i++) {
            m_head->data[i - 1] = m_head->data[i];
        }
    }
}