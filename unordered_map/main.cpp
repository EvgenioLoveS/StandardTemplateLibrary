#include <forward_list>
#include <vector>
#include <string>


template<class KeyType, class ValueType>
class Unordered_map {
    size_t _capacity = 10;
    std::forward_list<std::pair<KeyType, ValueType>> *buckets;

    KeyType rehash(KeyType key)
    {
        return key % 15;
    }

    KeyType getHash(KeyType key)
    {
        return key % 15;
    }

    void resize()
    {
        _capacity *= 2;
        auto newBuckets = new std::forward_list<std::pair<KeyType, ValueType>>[_capacity];
        for (int i = 0; i < _capacity / 2; i++)
        {
            for (std::pair<KeyType, ValueType> item: buckets[i])
            {
                newBuckets[rehash(item.first)].push_front(item);
            }
            if (!buckets[i].empty())
            {
                buckets[i].clear();
            }
        }
        //buckets->clear();
        delete[] buckets;
        buckets = newBuckets;
    }

public:
    Unordered_map()
    {
        buckets = new std::forward_list<std::pair<KeyType, ValueType>>[_capacity];
    }

    void insert(KeyType key, ValueType value)
    {
        while (_capacity < getHash(key)) resize();
        buckets[getHash(key)].push_front(std::make_pair(key, value));
    }

    std::string find(KeyType key)
    {
        int index = getHash(key);
        if (index >= _capacity) return "Empty";

        for (std::pair<KeyType, ValueType> item: buckets[index])
        {
            if (item.first == key) return item.second;
        }
        return "Empty";
    }

    void erase(KeyType key)
    {
        int index = getHash(key);
        if (index > _capacity) return;

        for (std::pair<KeyType, ValueType> item: buckets[index])
        {
            if (item.first == key)
            {
                buckets[index].remove(item);
                return;
            }
        }
    }

    void Clear()
    {
        for (int i = 0; i < _capacity; i++)
        {
            buckets[i].clear();
        }
        buckets->clear();
    }

    bool Contains(KeyType key)
    {
        std::string answ = find(key);
        return answ != "Empty" ? true : false;
    }

    ValueType &operator[](KeyType key)
    {
        int index = getHash(key);
        std::string answ = find(key);
        if (answ == "Empty")
        {
            insert(key, "Default");
        }
        typename std::forward_list<std::pair<KeyType, ValueType>>::iterator it;
        for (it = buckets[index].begin(); it != buckets[index].end(); ++it)
        {
            if ((*it).first == key)
            {
                std::pair<KeyType, ValueType> tmp = *it;
                return (*it).second;
            }
        }
    }

    ValueType operator[](KeyType key) const
    {
        int index = getHash(key);
        std::string answ = find(key);
        if (answ == "Empty")
        {
            insert(key, "Default");
        }
        typename std::forward_list<std::pair<KeyType, ValueType>>::iterator it;
        for (it = buckets[index].begin(); it != buckets[index].end(); ++it)
        {
            if ((*it).first == key)
            {
                std::pair<KeyType, ValueType> tmp = *it;
                return (*it).second;
            }
        }
    }

    std::string ToString()
    {
        std::string result;
        for (int i = 0; i < _capacity; i++)
        {
            for (const auto& pair : buckets[i])
            {
                result += "(" + std::to_string(pair.first) + ", " + std::to_string(pair.second) + ") ";
            }
        }
        return result;
    }

    std::forward_list<std::pair<KeyType, ValueType>> ReturnList(int key)
    {
        return buckets[key];
    }

    int GetSize()
    {
        return _capacity;
    }
};

