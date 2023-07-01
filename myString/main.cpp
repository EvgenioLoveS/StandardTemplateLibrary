#include <stdexcept>
#include <memory>

class MyUniquePtr {

private:
    char* _ptr;

public:

    MyUniquePtr() : _ptr(nullptr) {}
    explicit MyUniquePtr(char* unObj) : _ptr(unObj) {}
    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr(MyUniquePtr&& other) : _ptr(other.release()) {}

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

    char& operator*() const noexcept {
        return *_ptr;
    }
    char* operator->() const noexcept {
        return _ptr;
    }

    char* get() const {
        return _ptr;
    }
    void reset() {
        delete _ptr;
        _ptr = nullptr;
    }
    void reset(char* ptr) {
        if (_ptr != ptr) {
            reset();
            _ptr = ptr;
        }
    }
    char* release() {
        char* ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    }

    operator bool() const noexcept { return static_cast<bool>(_ptr); }

    MyUniquePtr make_unique(char value) {
        return MyUniquePtr(new char(value));
    }
};

class MyIteratorString {

private:
    char* _ptr;

public:

    MyIteratorString() { };
    MyIteratorString(char* ptr) : _ptr(ptr) { }
    ~MyIteratorString() { }

    MyIteratorString& operator++() {
        ++_ptr;
        return *this;
    }

    MyIteratorString operator++(int) {
        MyIteratorString tmp = *this;
        ++_ptr;
        return tmp;
    }

    MyIteratorString& operator--() {
        --_ptr;
        return *this;
    }

    MyIteratorString operator--(int) {
        MyIteratorString tmp = *this;
        --_ptr;
        return tmp;
    }

    char& operator*() const {
        return *_ptr;
    }

    char* operator->() const {
        return _ptr;
    }

    bool operator==(const MyIteratorString& other) const {
        return _ptr == other._ptr;
    }

    bool operator!=(const MyIteratorString& other) const {
        return _ptr != other._ptr;
    }

    MyIteratorString& operator=(const MyIteratorString& tmp) {
        if (this != &tmp) {
            _ptr = tmp._ptr;
        }
        return *this;
    }

    operator char*() const {
        return _ptr;
    }

    MyIteratorString& begin();
    MyIteratorString& end();
};

class MyString : public MyIteratorString {

private:
    MyUniquePtr _data;
    size_t _size;

public:

    MyString() : _size(0) {
        _data = MyUniquePtr(new char[1]);
        _data.get()[0] = '\0';
    }

    MyString(const char *str) : _size(mystrlen(str)) {
        _data = MyUniquePtr(new char[_size + 1]);
        mystrcpy(_data.get(), str);
    }

    MyString(const char *str, size_t count) :
            _size(count),
            _data(MyUniquePtr(new char[_size + 1]))
    {
        mystrncpy(_data.get(), str, _size);
        _data.get()[_size] = '\0';
    }

    MyString(const MyString &other) : MyIteratorString(other), _size(other._size) {
        _data = MyUniquePtr(new char[_size + 1]);
        mystrcpy(_data.get(), other._data.get());
        _data.get()[_size] = '\0';
    }

    ~MyString() = default;

    const char *getData() const {
        return _data.get();
    }

    const char *c_str() const {
        return _data.get();
    }

    char& operator[](int ind) {
        return _data.get()[ind];
    }

    const char& operator[](int ind) const {
        return _data.get()[ind];
    }

    MyString &operator=(const MyString &other) {
        if (this != &other) {
            _size = other._size;
            _data = MyUniquePtr(new char[_size + 1]);
            mystrcpy(_data.get(), other._data.get());
        }
        return *this;
    }

    MyString &operator=(MyString &&other) {
        if (this != &other) {
            _size = other._size;
            _data = std::move(other._data);
            other._size = 0;
        }
        return *this;
    }

    MyString &operator=(const char *str) {
        size_t len = mystrlen(str);
        _size = len;
        _data = MyUniquePtr(new char[_size + 1]);
        mystrcpy(_data.get(), str);
        return *this;
    }

    MyString &operator+=(const MyString &other) {
        int new_size = _size + other._size;
        char *new_data = new char[new_size + 1];
        mystrcpy(new_data, _data.get());
        mystrcat(new_data, other._data.get());
        _data = MyUniquePtr(new_data);
        _size = new_size;
        return *this;
    }

    MyString operator+(const MyString &other) const {
        MyString result(*this);
        result += other;
        return result;
    }

    MyIteratorString begin() {
        return MyIteratorString(_data.get());
    }

    MyIteratorString end() {
        return MyIteratorString(_data.get() + _size);
    }

    static const size_t npos = static_cast<size_t>(-1);

//-------------------------------------------------------------------------------
//Функции С++
//-------------------------------------------------------------------------------

    int size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void clear() {
        _data = MyUniquePtr(new char[1]);
        _data.get()[0] = '\0';
        _size = 0;
    }

    void resize(size_t newSize) {
        if (newSize == _size) {
            return;
        } else if (newSize < _size) {
            _data.get()[newSize] = '\0';
        } else {
            MyUniquePtr newData(new char[newSize + 1]);
            mymemcpy(newData.get(), _data.get(), _size);
            newData.get()[_size] = '\0';
            _data = std::move(newData);
        }
        _size = newSize;
    }

    void push_back(char c) {

        MyUniquePtr newData(new char[_size + 2]);

        mystrcpy(newData.get(), _data.get());

        newData.get()[_size] = c;
        newData.get()[_size + 1] = '\0';

        _data = std::move(newData);

        _size += 1;
    }

    MyString& insert(size_t start, const MyString &s) {
        if (start > _size)
            throw std::out_of_range("Index out of range");

        int newSize = _size + s._size;
        MyUniquePtr newData(new char[newSize + 1]);
        mystrncpy(newData.get(), _data.get(), start);
        mystrncpy(newData.get() + start, s._data.get(), s._size);
        mystrncpy(newData.get() + start + s._size, _data.get() + start, _size - start);
        newData.get()[newSize] = '\0';

        _data = std::move(newData);
        _size = newSize;

        return *this;
    }

    MyString& insert(size_t start, const MyString &s, size_t insStart, size_t num) {
        if (start > _size)
            throw std::out_of_range("Index out of range");

        if (insStart >= s._size)
            throw std::out_of_range("Index out of range");

        size_t newSize = _size + num;
        MyUniquePtr newData(new char[newSize + 1]);
        mystrncpy(newData.get(), _data.get(), start);
        mystrncpy(newData.get() + start, s._data.get() + insStart, num);
        mystrncpy(newData.get() + start + num, _data.get() + start, _size - start);
        newData.get()[newSize] = '\0';

        _data = std::move(newData);
        _size = newSize;

        return *this;
    }

    MyString& replace(size_t start, size_t num, const MyString &s) {
        if (start > _size)
            throw std::out_of_range("Index out of range");

        size_t newSize = _size - num + s._size;
        MyUniquePtr newData(new char[newSize + 1]);
        mystrncpy(newData.get(), _data.get(), start);
        mystrncpy(newData.get() + start, s._data.get(), s._size);
        mystrncpy(newData.get() + start + s._size, _data.get() + start + num, _size - start - num);
        newData.get()[newSize] = '\0';

        _data = std::move(newData);
        _size = newSize;

        return *this;
    }

    MyString& replace(size_t start, size_t num, const MyString &s, size_t replStart, size_t replNum) {
        if (start > _size)
            throw std::out_of_range("Index out of range");

        if (replStart >= s._size)
            throw std::out_of_range("Index out of range");

        size_t newSize = _size - num + replNum;
        MyUniquePtr newData(new char[newSize + 1]);
        mystrncpy(newData.get(), _data.get(), start);
        mystrncpy(newData.get() + start, s._data.get() + replStart, replNum);
        mystrncpy(newData.get() + start + replNum, _data.get() + start + num, _size - start - num);
        newData.get()[newSize] = '\0';

        _data = std::move(newData);
        _size = newSize;

        return *this;
    }

    MyString& erase(size_t index, size_t num) {
        if (index >= _size)
            throw std::out_of_range("Index out of range");

        if (num == npos) {
            _size = index;
            _data.get()[_size] = '\0';
        } else if (num > 0) {
            size_t newSize = _size - num;
            mystrncpy(_data.get() + index, _data.get() + index + num, _size - index - num);
            _data.get()[newSize] = '\0';
            _size = newSize;
        }

        return *this;
    }

    MyString& append(const MyString &s, size_t start) {
        size_t sLen = s.size();
        size_t len = size();
        if (start > len) {
            return *this;
        }
        if (sLen == 0) {
            return *this;
        }
        MyUniquePtr newBuffer(new char[len + sLen]);
        char *newData = newBuffer.get();
        const char *data = _data.get();
        const char *sData = s._data.get();

        for (size_t i = 0; i < len; ++i) {
            newData[i] = data[i];
        }

        mymemcpy(newData + len, sData + start, sLen);

        _data = std::move(newBuffer);
        _size = len + sLen;

        return *this;
    }

    MyString& append(const char *s, size_t num) {
        size_t len = size();
        if (num == 0) {
            return *this;
        }
        MyUniquePtr newBuffer(new char[len + num]);
        char *newData = newBuffer.get();
        const char *data = _data.get();

        for (size_t i = 0; i < len; ++i) {
            newData[i] = data[i];
        }

        for (size_t i = 0; i < num; ++i) {
            newData[len + i] = s[i];
        }

        _data = std::move(newBuffer);
        _size = len + num;

        return *this;
    }

    MyString& assign(void) {
        MyUniquePtr newBuffer(new char[1]{'\0'});
        _data = std::move(newBuffer);
        _size = 0;
        return *this;
    }

    MyString& assign(const MyString &s, size_t st, size_t num) {
        size_t sLen = s.size();
        if (st >= sLen) {
            return *this;
        }
        num = (num < sLen - st) ? num : (sLen - st);
        MyUniquePtr newBuffer(new char[num]);
        char *newData = newBuffer.get();
        const char *sData = s._data.get();

        mymemcpy(newData, sData + st, num);

        _data = std::move(newBuffer);
        _size = num;

        return *this;
    }

    size_t find(const MyString &s, size_t start) const {
        size_t sLen = s.size();
        size_t len = size();
        if (start > len) {
            return npos;
        }
        if (sLen == 0) {
            return start;
        }
        const char *data = _data.get();
        const char *sData = s._data.get();
        for (size_t i = start; i <= len - sLen; ++i) {
            bool found = true;
            for (size_t j = 0; j < sLen; ++j) {
                if (*(data + i + j) != *(sData + j)) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return i;
            }
        }
        return npos;
    }


    int compare(size_t start, size_t num, const MyString &s) const {
        size_t len = size();
        size_t sLen = s.size();
        if (start > len) {
            throw std::out_of_range("Start position is out of range.");
        }
        size_t cmpLen = std::min(num, sLen);
        const char *data = _data.get();
        const char *sData = s._data.get();
        for (size_t i = 0; i < cmpLen; ++i) {
            if (*(data + start + i) < *(sData + i)) {
                return -1;
            } else if (*(data + start + i) > *(sData + i)) {
                return 1;
            }
        }
        if (cmpLen == sLen) {
            return 0;
        } else if (cmpLen == num) {
            return (sLen < num) ? 1 : -1;
        } else {
            return 0;
        }
    }
    
//-------------------------------------------------------------------------------
//Функции Си
//-------------------------------------------------------------------------------

    void *mymemcpy(void *s1, const void *s2, size_t n) {
        if (s1 == nullptr) return nullptr;

        char *p = static_cast<char *>(s1);
        const char *q = static_cast<const char *>(s2);

        std::unique_ptr<char[]> buffer(new char[n]);

        memcpy(buffer.get(), q, n);
        memcpy(p, buffer.get(), n);

        return s1;
    }

    void *mymemmove(void *s1, const void *s2, size_t n) {
        char *d = static_cast<char *>(s1);
        const char *s = static_cast<const char *>(s2);

        if (d == s) {
            return s1;
        }

        if (d < s) {
            std::unique_ptr<char[]> buffer(new char[n]);

            memcpy(buffer.get(), s, n);
            memcpy(d, buffer.get(), n);
        } else {
            std::unique_ptr<char[]> buffer(new char[n]);

            for (size_t i = n; i > 0; --i) {
                buffer[i - 1] = s[i - 1];
            }

            for (size_t i = 0; i < n; ++i) {
                d[i] = buffer[i];
            }
        }

        return s1;
    }

    void *mymemset(void *s, int c, size_t n) {
        auto p = static_cast<char *>(s);

        std::unique_ptr<char[]> s_copy(new char[n]);

        for (size_t i = 0; i < n; i++) {
            s_copy[i] = static_cast<char>(c);
        }

        mymemcpy(s, s_copy.get(), n);

        return s;
    }

    size_t mystrlen(const char *s) {
        size_t len = 0;
        std::unique_ptr<const char[]> ptr{s};

        while (*s != '\0') {
            len++;
            s++;
        }

        return len;
    }

    int mystrcmp(const char *s1, const char *s2) {
        int n = 1000;

        std::unique_ptr<char[]> ps11(new char[n]);
        for (size_t i = n; i > 0; --i) {
            ps11[i - 1] = s1[i - 1];
        }

        std::unique_ptr<char[]> ps22(new char[n]);
        for (size_t i = n; i > 0; --i) {
            ps22[i - 1] = s2[i - 1];
        }

        int i = 0;
        while (ps11[i] != '\0' || ps22[i] != '\0') {
            if (ps11[i] < ps22[i]) {
                return -1;
            } else if (ps11[i] > ps22[i]) {
                return 1;
            }
            ps11[i + 1];
            ps22[i + 1];
        }

        return 0;
    }

    int mystrncmp(const char *s1, const char *s2, size_t n) {
        std::unique_ptr<char[]> ps1(new char[n]);
        for (size_t i = n; i > 0; --i) {
            ps1[i - 1] = s1[i - 1];
        }

        std::unique_ptr<char[]> ps2(new char[n]);
        for (size_t i = n; i > 0; --i) {
            ps2[i - 1] = s2[i - 1];
        }

        int i = 0;
        while ((ps1[i] != '\0' || ps2[i] != '\0') && n > 0) {
            if (ps1[i] < ps2[i]) {
                return -1;
            } else if (ps1[i] > ps2[i]) {
                return 1;
            }
            ps1[i + 1];
            ps2[i + 1];
            n--;
        }
        return 0;
    }

    char *mystrcat(char *s1, const char *s2) {
        size_t s1Length = mystrlen(s1);
        size_t s2Length = mystrlen(s2);

        std::unique_ptr<char[]> result(new char[s1Length + s2Length + 1]);

        // Копируем символы из s1 в result
        memcpy(result.get(), s1, s1Length);

        memcpy(result.get() + s1Length, s2, s2Length + 1);

        return result.release();
    }

    char *mystrncat(char *s1, const char *s2, size_t n) {

        size_t s1Length = mystrlen(s1);

        std::unique_ptr<char[]> result(new char[s1Length + n + 1]);

        mymemcpy(result.get(), s1, s1Length);

        mystrncpy(result.get() + s1Length, s2, n);
        result.get()[s1Length + n] = '\0';

        return result.release();
    }

    int mymemcmp(const void *s1, const void *s2, size_t n) {

        const char *d = static_cast<const char *>(s1);
        const char *s = static_cast<const char *>(s2);

        std::unique_ptr<char[]> ps1(new char[n]);
        for (size_t i = n; i > 0; --i) {
            ps1[i - 1] = d[i - 1];
        }

        std::unique_ptr<char[]> ps2(new char[n]);
        for (size_t i = n; i > 0; --i) {
            ps2[i - 1] = s[i - 1];
        }

        int i = 0;

        for (size_t i = 0; i < n; i++) {
            if (ps1[i] < ps2[i]) {
                return -1;
            } else if (ps1[i] > ps2[i]) {
                return 1;
            }
            ps1[i++];
            ps2[i++];
        }

        return 0;
    }

    char *mystrcpy(char *s1, const char *s2) {
        char *dest = s1;

        size_t s2Length = mystrlen(s2);

        std::unique_ptr<char[]> result(new char[s2Length + 1]);

        mymemcpy(result.get(), s2, s2Length);

        result[s2Length] = '\0';

        mymemcpy(s1, result.get(), s2Length + 1);

        return dest;
    }

    char *mystrncpy(char *s1, const char *s2, size_t n) {
        char *dest = s1;

        std::unique_ptr<char[]> result(new char[n + 1]);

        for (size_t i = 0; i < n && s2[i] != '\0'; i++) {
            result[i] = s2[i];
        }

        for (size_t i = n; i > 0; i--) {
            if (s2[i - 1] == '\0') {
                result[i - 1] = '\0';
            }
        }

        for (size_t i = 0; i < n; i++) {
            s1[i] = result[i];
        }

        return dest;
    }

    char *mystrchr(const char *str, int ch) {
        while (*str != '\0') {
            if (*str == ch) {
                return const_cast<char *>(str);
            }
            ++str;
        }
        return nullptr;
    }

    char *mystrtok(char *s1, const char *s2) {
        static std::unique_ptr<char, void (*)(char *)> lastToken(nullptr, [](char *p) {});

        if (s1 != nullptr) {
            lastToken.reset(s1);
        } else {
            s1 = lastToken.get();
        }

        while (*s1 != '\0' && mystrchr(s2, *s1) != nullptr) {
            *s1++ = '\0';
        }

        if (*s1 == '\0') {
            return nullptr;
        }

        char *token = s1;

        while (*s1 != '\0' && mystrchr(s2, *s1) == nullptr) {
            s1++;
        }

        if (*s1 != '\0') {
            *s1++ = '\0';
        }

        lastToken.reset(s1);
        return token;
    }
};

