#include <stdexcept>

#ifndef CPU_STACK_H
#define CPU_STACK_H


class stack {
private:
    size_t capacity = 0, current = -1, idx;

    char* data = nullptr;

    void redouble() {
        char* tmp = new char[capacity << 1];
        for (idx = 0; idx < capacity; idx++)tmp[idx] = data[idx];
        std::swap(data, tmp);
        delete[]tmp;
        capacity <<= 1;
    }

public:
    stack(const size_t capacity = 256) {
        this->capacity = capacity;
        data = new char[capacity];
    }

    ~stack() {
        delete[]data;
        data = nullptr;
        capacity = 0, current = -1;
    }

    char top() {
        if (current < 0)
            throw std::logic_error("stack error: index out of range");
        return data[current];
    }

    void push(const char ch) {
        if (current == capacity) {
            redouble();
        }
        data[++current] = ch;
    }

    char pop() {
        char tmp = top();
        data[current--] = 0;
        return tmp;
    }

    bool empty() const {
        return current == -1;
    }

    size_t size() const {
        return current + 1;
    }

};


#endif
