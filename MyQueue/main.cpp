#include <memory>

template <class T>
class MyUniquePtr {

private:
    T* _ptr;

public:
    MyUniquePtr() : _ptr(nullptr) { }
    explicit MyUniquePtr(T* unObj) : _ptr(unObj) { }
    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr(MyUniquePtr&& other) : _ptr(other.release()) { }

    ~MyUniquePtr() {
        reset();
    }

    MyUniquePtr& operator=(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(MyUniquePtr&& other) {
        if (this != &other) {
            reset();
            _ptr = other.release();
        }
        return *this;
    }

    T& operator*() const noexcept {
        return *_ptr;
    }

    T* operator->() const noexcept {
        return _ptr;
    }

    T* get() const {
        return _ptr;
    }

    void reset() {
        delete _ptr;
        _ptr = nullptr;
    }

    void reset(T* ptr) {
        if (_ptr != ptr) {
            reset();
            _ptr = ptr;
        }
    }

    T* release() {
        T* ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    }

    operator bool() const noexcept { return static_cast<bool>(_ptr); }
};


template <class T>
class MyQueue {

private:

    struct Node
    {
        Node(const T& value) : data(value), next(nullptr) { }
        T data;
        MyUniquePtr<Node> next;
    };

    MyUniquePtr<Node> m_head;
    Node* m_tail;
    size_t m_size;

public:

    MyQueue() : m_head(nullptr), m_tail(nullptr), m_size(0) { }

    virtual ~MyQueue();

    void push(const T& item);

    void pop();

    bool isEmpty() const {
        return m_size == 0;
    }

    int getSize() const {
        return m_size;
    }

    const T& front() const {
        return m_head->data;
    }

    const T& back() const {
        return m_tail->data;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};


template<class T>
MyQueue<T>::~MyQueue(){
    while(!isEmpty()){
        pop();
    }
}

template<class T>
void MyQueue<T>::push(const T& item){
    MyUniquePtr<Node> newNode(new Node(item));
    if (isEmpty()) {
        m_head = std::move(newNode);
        m_tail = m_head.get();
    } else {
        m_tail->next = std::move(newNode);
        m_tail = m_tail->next.get();
    }
    m_size++;
}

template<class T>
void MyQueue<T>::pop() {
    if (!isEmpty()) {
        m_head = std::move(m_head->next);
        m_size--;
        if (isEmpty()) {
            m_tail = nullptr;
        }
    }
}