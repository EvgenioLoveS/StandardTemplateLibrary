#include <cmath>
#include <stdexcept>

template <class T>
class MyVector {

private:
    size_t _size = 0;
    size_t _capacity = 1;
    T* v{};

public:

    MyVector() {
        v = new T[1];
    }

    ~MyVector() {
        delete[] v;
    }

    MyVector(size_t size, const T& value = T{}) : _size(size), _capacity(size == 0 ? 1 : size) {
        v = new T[_capacity];
        for (size_t i = 0; i < _size; ++i) {
            v[i] = value;
        }
    }

    MyVector(const MyVector& other) : _size(other._size), _capacity(other._capacity), v(new T[_capacity]) {
        std::copy(other.v, other.v + _size, v);
    }

    MyVector(std::initializer_list<T> list) : v(new T[list.size()]), _size(list.size()), _capacity(list.size()) {
        std::copy(list.begin(), list.end(), v);
    }

    class iterator {

    private:
        T* _ptr;

    public:

        iterator(T* vec): _ptr(vec) { };
        ~iterator() { };

        bool operator==(const iterator& tmp) const
        {
            return _ptr == tmp._ptr;
        };

        bool operator!=(const iterator& tmp) const
        {
            return !(* this == tmp);
        };

        iterator& operator=(const iterator &tmp)
        {
            _ptr=tmp._ptr;
        };

        iterator& operator++()
        {
            _ptr++;
            return *this;
        }

        iterator& operator--()
        {
            _ptr--;
            return *this;
        }

        T& operator*() const
        {
            return *_ptr;
        };

        T* operator->()
        {
            return _ptr;
        };

        T* operator&()
        {
            return _ptr;
        };
    };

    class reverse_iterator {

    private:
        T* _ptr;

    public:

        reverse_iterator(T* vec): _ptr(vec) { };
        ~reverse_iterator() { };

        bool operator==(const iterator& tmp) const
        {
            return _ptr == tmp._ptr;
        };

        bool operator!=(const iterator& tmp) const
        {
            return !(* this == tmp);
        };

        iterator& operator=(const iterator &tmp)
        {
            _ptr=tmp._ptr;
        };

        iterator& operator++()
        {
            _ptr--;
            return *this;
        }

        iterator& operator--()
        {
            _ptr++;
            return *this;
        }

        T& operator*() const
        {
            return *_ptr;
        };

        T* operator->()
        {
            return _ptr;
        };

        T* operator&()
        {
            return _ptr;
        };
    };

    T& operator[](size_t index)  {
        return v[index];
    }

    const T& operator[](size_t index) const {
        return v[index];
    }

    MyVector operator=(const MyVector& other) {
        if (this != &other) {
            delete[] v;
            v = new T[other._capacity];
            for(size_t i = 0; i < other._size; ++i) v[i] = other.v[i];
            _size = other._size;
            _capacity = other._size;
        }
        return *this;
    }

    size_t size() const {
        return _size;
    }

    unsigned long long max_size() {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    size_t capacity() const {
        return _capacity;
    }

    T* data() {
        return v;
    }

    void clear() {
        delete[] v;
        v = new T[1];
        _capacity = 1;
        _size = 0;
    }

    bool empty() {
        return _size == 0;
    }

    T& at(size_t i) {
        if(i > _size - 1){
            return *v;
        }
        return v[i];
    }

    T& back() {
        if(!_size){
            return *v;
        }
        return v[_size-1];
    }

    T& front() {
        return v[0];
    }

    void reserve(const size_t& n) {
        _capacity = n;
        T* tmp = v;
        v = new T[_capacity];

        if(_size > _capacity){
            _size = _capacity - 1;
        }

        for(int i = 0; i < _size; ++i){
            v[i] = tmp[i];
        }
    }

    void resize(const size_t& n) {
        if(!_capacity){
            _capacity = 1;
            while (_capacity < n) {
                _capacity *= 2;
            }
            v = new T[_capacity];
            for (size_t i = 0; i < n; ++i) v[i] = {};
            _size = n;
        }
        else{
            while(_capacity < n){
                _capacity *= 2;
            }
            T* tmp = v;
            v = new T[_capacity];
            for (size_t i = 0; i < _size; ++i) v[i] = tmp[i];
            for(size_t i = _size; i < n; ++i) v[i] = 0;
            _size = n;
            delete[] tmp;
        }
    }

    void push_back(const T& value) {
        if(!_capacity){
            _capacity = _size + 1;
            v = new T[_capacity];
        }
        else if (_size == _capacity){
            _capacity *= 2;
            T* tmp = v;
            v = new T[_capacity];
            for (size_t i = 0; i < _size; ++i) v[i] = tmp[i];
            delete[] tmp;
        }
        ++_size;
        v[_size-1] = value;
    }

    void pop_back() {
        if(!_size){
            return;
        }
        --_size;
        if(_capacity / 2 >= _size){
            _capacity /= 2;
            T* tmp = v;
            v = new T[_capacity];
            for(int i = 0; i < _size;i++) v[i] = tmp[i];
            delete[] tmp;
        }
    }

    void insert(const iterator& it, const T& value) {
        ++_size;
        size_t ind = it.base() - v;
        if(_size > _capacity){
            _capacity *= 2;
            T* tmp = v;
            v = new T[_capacity];
            for (size_t i = 0; i < ind; ++i) v[i] = tmp[i];
            v[ind] = value;
            for(size_t i = ind + 1; i < _size; ++i) v[i] = tmp[i-1];
            delete[] tmp;
        }
        else{
            for(size_t i = _size; i > ind; --i){
                v[i] = v[i-1];
            }
            v[ind] = value;
        }
    }

    void erase(const iterator& it) {
        int ind = it.base() - v;
        --_size;
        for(size_t i = ind; i < _size; ++i){
            v[i] = v[i+1];
        }
    }

    void assign(iterator start, iterator end) {
        delete[] v;
        v = nullptr;
        _size = 0;
        _capacity = 1;
        iterator tmp = start;
        while(tmp != end){
            tmp++;
            ++_size;
            if(_size > _capacity){
                _capacity *= 2;
            }
        }
        v = new T[_capacity];
        for(int i = 0; start <= end; ++start, ++i){
            v[i] = *start.base();
        }
    }

    void swap(MyVector& other) {
        if(this != &other){
            MyVector<T> tmp(other);
            other = *this;
            *this = tmp;
        }
    }

    iterator begin() {
        return iterator(v);
    }

    iterator end() {
        if(!_size){
            return nullptr;
        }
        return iterator(&v[_size]);
    }

    const iterator cbegin() const {
        return iterator(v);
    }


    const iterator cend() const {
        if(!_size){
            return nullptr;
        }
        return iterator(&v[_size]);
    }

    reverse_iterator rend() {
        return reverse_iterator(v-1);
    }

    reverse_iterator rbegin() {
        if(!_size){
            return nullptr;
        }
        return reverse_iterator(v + _size - 1);
    }

    const reverse_iterator crend() const {
        return iterator(v);
    }

    const reverse_iterator crbegin() const {
        if(!_size){
            return nullptr;
        }
        return iterator(&v[_size]);
    }

    template<class... Args>
    _COMPLEX_DEFINED void emplace_back(Args&&... args) noexcept {
        using expander = T[];
        (void)expander{NULL,
                       (void(push_back(args)), NULL)...};
    }

    template<class... Args>
    void emplace(const iterator& it, Args&&... args) noexcept {
        size_t pos = it.base() - v;
        size_t oldSize = _size;
        T* newV = new T[_size - pos];
        for(size_t i = pos - 1, j = 0; i < _size; ++i, ++j){
            newV[j] = v[i];
        }
        _size -= pos;
        emplace_back(args...);
        for(size_t i = 0; i < oldSize - pos + 1; ++i){
            push_back(newV[i]);
        }
        delete[] newV;
    }
};

