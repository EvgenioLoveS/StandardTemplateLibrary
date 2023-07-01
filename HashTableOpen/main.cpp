#include <iostream>

class HashTableOpen {

private:
    const int SIZE;
    int* keys;
    std::string* values;

public:
    HashTableOpen(int size) : SIZE(size) {
        keys = new int[SIZE];
        values = new std::string[SIZE];
        for (int i = 0; i < SIZE; i++) {
            keys[i] = -1;
            values[i] = "";
        }
    }

    ~HashTableOpen() {
        delete[] keys;
        delete[] values;
    }

    int hashFunction(int key) {
        int i = 0;
        int h_ = key % SIZE;

        return (h_ + i) % SIZE;
    }

    void insert(int key, std::string value) {
        int index = hashFunction(key);
        while (keys[index] != -1) {
            index = (index + 1) % SIZE;
        }
        keys[index] = key;
        values[index] = value;
    }

    std::string search(int key) {
        int index = hashFunction(key);
        while (keys[index] != -1) {
            if (keys[index] == key) {
                return values[index];
            }
            index = (index + 1) % SIZE;
        }
        return "";
    }

    void remove(int key) {
        int index = hashFunction(key);
        while (keys[index] != -1) {
            if (keys[index] == key) {
                keys[index] = -1;
                values[index] = "";
                return;
            }
            index = (index + 1) % SIZE;
        }
    }
};
