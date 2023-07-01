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
class MySharedPtr {

private:
    T* _ptr;
    int* counter;

public:
    MySharedPtr() :
        _ptr(nullptr), counter(nullptr) { }

    explicit MySharedPtr(T* ptr)
        : _ptr(ptr), counter(new int(1)) { };

    MySharedPtr(const MySharedPtr& other) noexcept
        : _ptr(other._ptr), counter(other.counter) {
        if(counter != nullptr){
            ++*counter;
        }
    }

    MySharedPtr(MySharedPtr&& other) noexcept
        : _ptr(other._ptr), counter(other.counter) {
        other._ptr = nullptr;
        other.counter = nullptr;
    }

    ~MySharedPtr() {
        if(!counter) return;
        --*counter;
        if(*counter == 0){
            delete _ptr;
            delete counter;
        }
    }

    T* operator->() const noexcept { return _ptr; }

    T& operator*() const noexcept { return *_ptr; }

    unsigned use_counter() const noexcept { return (counter) ? *counter : 0; }

    MySharedPtr& operator=(const MySharedPtr& other) noexcept {
        if(this == &other) return *this;
        if(counter != nullptr){
            --*counter;
            if(*counter == 0){
                delete _ptr;
                delete counter;
            }
        }
        _ptr = other._ptr;
        counter = other.counter;
        if(counter != nullptr){
            ++*counter;
        }
        return *this;
    }
};
